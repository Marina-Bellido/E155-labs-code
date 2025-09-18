//------------------------------------------------------------------------------
// Author:   Marina Bellido (mbellido@hmc.edu)
// Date:     18/9/2025
// File:     fsm_mb.sv
//=============================================================
// Description:
// This module implements a finite state machine (FSM) to control 
// keypad scanning, debouncing, key storage, and display updates. 
// It coordinates between detecting key presses, waiting for 
// debounce time, writing the detected value, and enabling the 
// 7-segment display.
//
// States:
//   S0 (SCAN)     - Scan keypad rows/cols for a press
//   S1 (SAVE)     - A key was detected, stop scanning
//   S2 (WAIT)     - Wait for debounce timer (time_passed)
//   S3 (CHECK)    - Confirm if key is still pressed
//   S4 (DISPLAY)  - Update the display with new key value
//   S5 (HOLD)     - Wait until the key is released
//=============================================================

module fsm_mb(
	input logic reset, int_osc,           // Reset and internal oscillator clock
	input logic pushed, time_passed,      // Push detection and debounce timer pulse
	output logic stop_counter,            // Controls row scanning counter
	output logic display_enable,          // Enables 7-segment update
	output logic write_enable,            // Enables key storage
	output logic debounce_enable);        // Enables debounce timing
	

	// === State encoding ===
	typedef enum logic [2:0] {S0, S1, S2, S3, S4, S5} statetype;
	statetype state, nextstate;

	// === State register ===
	always_ff @(posedge int_osc, negedge reset) begin
		if (reset == 0) state <= S0;      // Reset → go to SCAN
		else state <= nextstate;
	end

	// === Next State Logic ===
	always_comb begin
		case(state)
			S0: if(pushed) nextstate = S1; 	      // Key detected → SAVE
				else nextstate = S0;              // Keep scanning
			S1: nextstate = S2; 			      // Move to WAIT
			S2: if(time_passed) nextstate = S3;   // Wait until debounce finishes
				else nextstate = S2;
			S3: if(pushed) nextstate = S4;  	  // If still pressed → DISPLAY
				else nextstate = S0;              // Else restart scanning
			S4: nextstate = S5; 			      // Show value
			S5: if(!pushed) nextstate = S0;       // Release detected → restart
				else nextstate = S5;              // Keep holding
		endcase
	end

	// === Output logic based on current state ===
	always_comb begin
		case(state)
			S0: begin
				stop_counter    = 0; // Allow row scanning
				display_enable  = 0;
				write_enable    = 0;
				debounce_enable = 0;
			end 
			S1: begin
				stop_counter    = 1; // Stop scanning (key found)
				display_enable  = 0;
				write_enable    = 0;
				debounce_enable = 0;
			end 
			S2: begin
				stop_counter    = 1; // Hold scanning
				display_enable  = 0;
				write_enable    = 0;
				debounce_enable = 1; // Enable debounce timer
			end 
			S3: begin
				stop_counter    = 1;
				display_enable  = 0;
				write_enable    = 1; // Store key value
				debounce_enable = 0;
			end 
			S4: begin
				stop_counter    = 1;
				display_enable  = 1; // Enable display update
				write_enable    = 0;
				debounce_enable = 0;
			end 
			S5: begin
				stop_counter    = 1;
				display_enable  = 0;
				write_enable    = 0;
				debounce_enable = 0; // Wait for release
			end 
		endcase
	end
	
endmodule 


//=============================================================
// FSM Summary:
// - S0: Scan rows/cols for a pressed key
// - S1: Key detected → stop scanning
// - S2: Wait for debounce interval
// - S3: Check if key still pressed and write_enable =1
// - S4: Update 7-segment display
// - S5: Wait until key released
//
// pushed      -> High when a key is pressed
// time_passed -> High when debounce timer completes
//=============================================================
