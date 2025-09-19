//------------------------------------------------------------------------------
// Author:   Marina Bellido (mbellido@hmc.edu)
// Date:     18/9/2025
// File:     lab3_mb.sv
//------------------------------------------------------------------------------
// Description:
// Top-level module that interfaces a keypad with two 7-segment 
// displays. It scans the keypad, debounces button presses, and 
// stores the most recent two digits entered. These digits are 
// multiplexed across the displays, showing the previous input 
// on one display and the current input on the other.
// -----------------------------------------------------------------------------

module lab3_mb(
	input  logic reset,              // Active-low reset signal
	input  logic [3:0]cols,          // Column inputs from the keypad
	output logic [3:0] rows,         // Row outputs for keypad scanning
	output logic [1:0] sel,          // Display selector (to choose between 2 displays)
	output logic [6:0] segment);     // 7-segment display outputs
	
	// Internal signals
	logic clk_signal, pushed, time_passed, stop_counter, display_enable, write_enable, debounce_enable;
	logic [3:0] s;                   // Current digit to be sent to the 7-segment display driver
	logic [7:0] key_val;             // Key value read from the keypad
    logic [3:0] current_digit, prev_digit;  // Registers to store new and previous keypad values
	logic int_osc;                   // Internal oscillator clock signal

	// FSM controls sequencing of keypad scanning, debounce, and display
	fsm_mb states(reset, int_osc, pushed, time_passed, stop_counter, display_enable, write_enable, debounce_enable);

	// Handles keypad row scanning and detects pressed keys
	row_counter scanning(reset, int_osc, stop_counter, write_enable, cols, rows, key_val, pushed);

	// 7-segment display driver (converts digit to segment outputs)
	sevseg3 display(s, segment); 

	// Debouncer module to filter out switch bouncing
	debouncer deb(reset, int_osc, debounce_enable, time_passed);
	
	

	// ===================== Lab1 Starter Code =====================

	logic [23:0] counter; // 24-bit counter used for clock division
	
	// Internal high-speed oscillator primitive (from FPGA)
	HSOSC #(.CLKHF_DIV(2'b01)) 
		 hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));

	// Clock divider: generates clk_signal at ~60 Hz from high-speed int_osc
	always_ff @(posedge int_osc, negedge reset) begin
		if(reset == 0) begin
			counter <= 0;
			clk_signal <= 0;
		end else begin           
			if(counter == 23'd200_200) // Frequency divider target
				begin 
					counter <= 0;
					clk_signal <= ~clk_signal; // Toggle clk_signal
				end else begin
					counter <= counter + 1;
				end
		end 
	end 
			 
			 
	// Register logic: store previous and current keypad digits
	always_ff @(posedge int_osc or negedge reset) begin
		if (!reset) begin
			current_digit <= 4'd0;
			prev_digit    <= 4'd0;
		end else if (display_enable) begin
			prev_digit    <= current_digit;  // Move current into prev
			current_digit <= key_val;        // Update current with new input
		end
	end


	// Multiplexing between two 7-segment displays
	always_comb begin
        if (clk_signal == 0) begin
			sel = 2'b01;     // Enable display 1
			s = prev_digit;  // Show previous digit
		end else begin
			sel = 2'b10;     // Enable display 2
			s = current_digit; // Show current digit
		end
	end
	 
endmodule

