//------------------------------------------------------------------------------
// Author:   Marina Bellido (mbellido@hmc.edu)
// Date:     28/8/2025
// File:     lab1_mb.sv
//------------------------------------------------------------------------------
// Description:
//   Top-level module for Lab 1. 
//   - Drives LEDs and a 7-segment display based on the 4-bit input `s`.
//   - LED[0] = XOR of s[1] and s[0].
//   - LED[1] = AND of s[2] and s[3].
//   - LED[2] blinks at ~2.4 Hz using the internal high-speed oscillator (HSOSC).
//   - The 7-segment display is driven by the `sevseg` combinational module.
//------------------------------------------------------------------------------
// Inputs:
//   reset       : Active-low reset
//   s[3:0]      : 4-bit input vector
//
// Outputs:
//   led[2:0]    : LED outputs
//   segment[6:0]: 7-segment display outputs
//------------------------------------------------------------------------------

module lab1_mb(
    input  logic       reset, 
    input  logic [3:0] s,
    output logic [2:0] led, 
    output logic [6:0] segment
);

    //--------------------------------------------------------------------------
    // 7-segment display driver (combinational logic)
    //--------------------------------------------------------------------------
    sevseg d(s, segment); // Instantiate 7-segment driver

    //--------------------------------------------------------------------------
    // LED assignments (simple combinational logic)
    //--------------------------------------------------------------------------
    assign led[0] = s[1] ^ s[0]; // XOR of s[1] and s[0]
    assign led[1] = s[2] & s[3]; // AND of s[2] and s[3]

    //--------------------------------------------------------------------------
    // LED[2] blink logic (~2.4 Hz)
    //--------------------------------------------------------------------------
    logic int_osc;         // Internal oscillator clock
    logic [23:0] counter;  // Counter for LED toggle

    // Internal high-speed oscillator (48 MHz / 2 divider = 24 MHz)
    HSOSC #(.CLKHF_DIV(2'b01)) 
          hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));

    // Counter: toggles LED[2] every 5 million cycles (~2.4 Hz blink rate)
    always_ff @(posedge int_osc, negedge reset) begin
        if (reset == 0) begin
            counter <= 0;
            led[2]  <= 0;
        end else begin           
            if (counter == 23'd4_999_999) begin  
                counter <= 0;
                led[2]  <= ~led[2]; // Toggle LED
            end else begin
                counter <= counter + 1;
            end
        end
    end 

endmodule
