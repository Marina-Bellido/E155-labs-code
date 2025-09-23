//------------------------------------------------------------------------------
// Author:   Marina Bellido (mbellido@hmc.edu)
// Date:     18/9/2025
// File:     debouncer.sv
//=============================================================
// Description:
// This module generates a timing pulse used for debouncing 
// mechanical button inputs. When enabled, it counts clock cycles 
// and asserts `time_passed` once every ~0.1s (10Hz). 
// This prevents false multiple detections caused by switch bounce.
//=============================================================

module debouncer(
	input logic reset, int_osc,             // Active-low reset and clock input (clk_signal)
	input logic debounce_enable,        // Enables the debounce timing
	output logic time_passed);          // High for one cycle when debounce interval completes

	logic [23:0] counter;               // 24-bit counter for timing
	

    // Counter with debounce timing logic
	always_ff @(posedge int_osc, negedge reset) begin
		if(!reset) begin
			counter <= 0;
			time_passed <= 0;
		end else if (debounce_enable) begin    
			if(counter == 23'd120000) begin      // 10Hz -> ~0.1s delay at given clock
				counter <= 0;
				time_passed <= 1;                // Pulse high for one cycle
			end else begin 
				counter <= counter + 1;          // Keep counting
				time_passed <= 0;
			end
		end else begin 
			counter <= 0;                        // Disabled → reset everything
			time_passed <= 0;
		end
	end 

endmodule
