// Marina Bellido 
// mbellido@hmc.edu
// 18/9/2025
// FSM Testbench: Checking if we move through the different states created 

// Modelsim-ASE requires a timescale directive
`timescale 1ns / 1ns

module testbench_fsm();
    logic clk, reset;
    logic pushed, time_passed;
	logic write_enable, write_enable_expected;
	logic debounce_enable, debounce_enable_expected; 
	logic display_enable, display_enable_expected;
    logic stop_counter, stop_counter_expected;
	
	

    // Instantiate device under test
    fsm_mb dut(reset, clk, pushed, time_passed, stop_counter, display_enable, write_enable, debounce_enable);
	

	
    // Generate clock
    always begin
        clk = 1; #10; 
		clk = 0; #10;
    end
	
	initial begin
		reset = 0; #20 reset =1;
	end
		initial begin     
		#20
		pushed = 1;
		time_passed =0;
        #50;
		pushed = 1;
		time_passed =1;
        #50;
		pushed = 1;
		time_passed =1;
		
        #50;
		pushed = 0;

	end
	



	typedef enum logic [2:0] {S0, S1, S2, S3, S4, S5} 
	statetype;
	statetype state, nextstate;
	
	
	always_ff @(posedge clk) begin
			
		assert (write_enable == write_enable_expected)else $error("Assertion failed write_enable: %b %b",  write_enable,  write_enable_expected);
		assert (debounce_enable == debounce_enable_expected)else $error("Assertion failed debounce_enable: %b %b",  debounce_enable,  debounce_enable_expected);
		assert (display_enable == display_enable_expected)else $error("Assertion failed display_enable: %b %b", display_enable, display_enable_expected);
		assert (stop_counter == stop_counter_expected)else $error("Assertion failed stop_counter: %b %b",  stop_counter,  stop_counter_expected);
	end

	//state register
	always_ff @(posedge clk, posedge reset)
		if (!reset) state <= S0; 
		else state <= nextstate;
	//Next State Logic:	
	always_comb begin
		case(state)
			S0: if(pushed) nextstate = S1; 	    //SCANNING
				else nextstate = S0;
			S1: nextstate = S2; 			    //SAVE KEY
			S2: if(time_passed) nextstate = S3; //WAIT -> DEBOUNCER
				else nextstate = S2;
			S3: if(pushed) nextstate = S4;  	//CHECK if pushed
				else nextstate = S0;
			S4: nextstate = S5; 				//DISPLAY
			S5: if(!pushed) nextstate = S0;     //CHECK if pushed
				else nextstate = S5;
		endcase
		
	end


	//next state logic variables:
	always_comb begin
		case(state)
			S0: begin
				stop_counter_expected    = 0; //pretty sure this is 0 bc ur scanning and when you push and move to S1 than you stop
				display_enable_expected  = 0;
				write_enable_expected    = 0;
				debounce_enable_expected = 0;
			end 
			S1: begin
				stop_counter_expected    = 1; //stop since you found a col on
				display_enable_expected  = 0;
				write_enable_expected    = 1;
				debounce_enable_expected = 0;
			end 
			S2: begin
				stop_counter_expected    = 1;
				display_enable_expected  = 0;
				write_enable_expected    = 0;
				debounce_enable_expected = 1;
			end 
			S3: begin
				stop_counter_expected    = 1;
				display_enable_expected  = 0;
				write_enable_expected    = 0;
				debounce_enable_expected = 0;
			end 
			S4: begin
				stop_counter_expected    = 1;
				display_enable_expected  = 1;
				write_enable_expected    = 0;
				debounce_enable_expected = 0;
			end 
			S5: begin
				stop_counter_expected    = 1;
				display_enable_expected  = 0;
				write_enable_expected    = 0;
				debounce_enable_expected = 0;
			end 
		endcase
	end


	
endmodule
