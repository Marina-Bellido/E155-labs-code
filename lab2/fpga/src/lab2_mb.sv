//------------------------------------------------------------------------------
// Author:   Marina Bellido (mbellido@hmc.edu)
// Date:     2/9/2025
// File:     lab2_mb.sv
//------------------------------------------------------------------------------
// Description:
//   Top-level module for Lab 2. .
//   Display two independent hexadecimal numbers on a dual seven-segment display. 
//   - Use 2 DIP switch to provide the data for two hexadecimal numbers. 
//   - The 7-segment display is driven by the `sevseg` combinational module:
//         Sevseg is used to drive the cathodes for both digits on the display, 
//         which therefore must be wired for multiplexed operation. 
//   - LED[5:0] = display the sum of the numbers on five LEDs.
// -----------------------------------------------------------------------------


module lab2_mb(
    input logic reset, 
	input logic  [3:0]s1, s2,  // Two DIP switches input
    output logic [4:0]led, 
	output logic [1:0]sel,     // Selector between the two hex numbers on the display
	output logic [6:0]segment);
	  
	logic clk_signal;
	logic [3:0]s;

	assign led = s1+ s2;
	sevseg display1(s, segment); // 7 segment display (combinational logic) 
	


    //Lab1 starter code: Blink at 2.4Hz
   logic int_osc;
   logic [23:0] counter; //initializing counter
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
		 end
		 end
     end 
	 
    // Whenever the clk_signal changes, select a different display number and input:
	always_comb begin
        if (clk_signal == 0) begin
			sel = 2'b01;
			s = s1;
		end else begin
			sel = 2'b10;
			s = s2;
		end
	
	end
  
endmodule
