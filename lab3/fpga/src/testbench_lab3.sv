// Marina Bellido 
// mbellido@hmc.edu
// 18/9/2025
// TESTBENCH2: TODO EXPLANATION

// Modelsim-ASE requires a timescale directive
`timescale 1ns / 1ns

module testbenchlab3();
    logic clk, reset;
	logic clk_signal, display_enable;
    logic [3:0] cols, s;
	logic [7:0] prev_digit, current_digit, key_val;
    logic [3:0] rows, rows_expected;
	logic [1:0] sel, sel_expected;
    logic [6:0] segment, segment_expected;
	logic [7:0] counter; //creating an 8 bit bus: first 4 bits are s0 and the other s2.
	//By adding a 1 at each counter you will be testing all possible combinations


    // Instantiate device under test
    lab3_mb dut(reset, cols, rows, sel, segment);
	

	
    // Generate clock
    always begin
        clk = 1; #10; 
		clk = 0; #10;
    end
	
	initial begin
		reset = 0; #10 reset =1;
	end
	
	initial begin     
		#20

	//drive the rows
		cols = 4'b0000;
		
		#200
        cols = 4'b0001;
		
		
	end
	

		
	//Lab1 starter code
	   logic int_osc;
	
	   // Internal high-speed oscillator
	   HSOSC #(.CLKHF_DIV(2'b01)) 
			 hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
	   // Counter
	   always_ff @(posedge int_osc, negedge reset) begin
			 if(reset == 0) begin
				 counter <= 0;
				 clk_signal <= 0;
			 end else begin           
				 if(counter == 23'd200_200) // 60Hz cut off frequency humans can no longer detect
					 begin 
					 counter <= 0;
					 clk_signal <= ~clk_signal; //flip the LED
				 end else begin
					counter <= counter + 1;
					assert (rows == rows_expected)else $error("Assertion failed rows: %b %b", rows, rows_expected);
					assert (sel == sel_expected)else $error("Assertion failed sel: %b %b", sel, sel_expected);
					assert (segment == segment_expected)else $error("Assertion failed segment: %b %b", segment, segment_expected);
				 end
				 end
			 end 
			 
			
	
	
			 
	  always_ff @(posedge clk or negedge reset) begin
			if (!reset) begin
				current_digit <= 4'd0;
				prev_digit    <= 4'd0;
			end else if (display_enable) begin
				prev_digit    <= current_digit;  // shift old to prev
				current_digit <= key_val;        // new digit stored
			end
		end


		 always_comb begin
			if (clk_signal == 0) begin
				sel_expected = 2'b01;
				s = current_digit;
			end else begin
				sel_expected = 2'b10;
				s = prev_digit;
			end
		
		end
	
		//clock can't be tested here so what im doing is using my sel to check what segment was selected:
		always_comb begin
			case(s)
				4'd0:  segment_expected = 7'b1000000; // 0: A,B,C,D,E,F ON; G OFF
				4'd1:  segment_expected = 7'b1111001; // 1: B,C
				4'd2:  segment_expected = 7'b0100100; // 2: A,B,G,E,D
				4'd3:  segment_expected = 7'b0110000; // 3: A,B,C,G,D
				4'd4:  segment_expected = 7'b0011001; // 4: B,C,F,G
				4'd5:  segment_expected = 7'b0010010; // 5: A,C,D,F,G
				4'd6:  segment_expected = 7'b0000010; // 6: A,C,D,E,F,G
				4'd7:  segment_expected = 7'b1111000; // 7: A,B,C
				4'd8:  segment_expected = 7'b0000000; // 8: all segments ON
				4'd9:  segment_expected = 7'b0010000; // 9: A,B,C,D,F,G
				4'd10: segment_expected = 7'b0001000; // A: A,B,C,E,F,G
				4'd11: segment_expected = 7'b0000011; // B: C,D,E,F,G
				4'd12: segment_expected = 7'b1000110; // C: A,D,E,F
				4'd13: segment_expected = 7'b0100001; // D: B,C,D,E,G
				4'd14: segment_expected = 7'b0000110; // E: A,D,E,F,G
				4'd15: segment_expected = 7'b0001110; // F: A,E,F,G
				default: segment_expected = 7'b1111111; // blank
			endcase
		end
		
endmodule
