//------------------------------------------------------------------------------
// Author:   Marina Bellido (mbellido@hmc.edu)
// Date:     28/8/2025
// File:     sevseg.sv
//------------------------------------------------------------------------------
// Description:
//   Combinational logic module for a common-anode 7-segment display.
//   - Maps a 4-bit input `s` (0–15) to a 7-bit output `segment`.
//   - Supports hexadecimal digit display (0–9, A–F).
//   - Outputs use active-low encoding: '0' = LED ON, '1' = LED OFF.
//------------------------------------------------------------------------------
// Inputs:
//   s[3:0]       : 4-bit input representing the value to display
//
// Outputs:
//   segment[6:0] : Active-low 7-segment signals 
//                  [A,B,C,D,E,F,G] = segment[6:0]
//------------------------------------------------------------------------------

module sevseg(
    input  logic [3:0] s,
    output logic [6:0] segment
);

    logic [6:0] seg;

    //--------------------------------------------------------------------------
    // Combinational logic: 4-bit input -> 7-segment pattern
    //--------------------------------------------------------------------------
    always_comb begin
        case (s)
            4'd0 : seg = 7'b1000000; // 0: A,B,C,D,E,F
            4'd1 : seg = 7'b1111001; // 1: B,C
            4'd2 : seg = 7'b0100100; // 2: A,B,G,E,D
            4'd3 : seg = 7'b0110000; // 3: A,B,C,G,D
            4'd4 : seg = 7'b0011001; // 4: B,C,F,G
            4'd5 : seg = 7'b0010010; // 5: A,C,D,F,G
            4'd6 : seg = 7'b0000010; // 6: A,C,D,E,F,G
            4'd7 : seg = 7'b1111000; // 7: A,B,C
            4'd8 : seg = 7'b0000000; // 8: all segments
            4'd9 : seg = 7'b0010000; // 9: A,B,C,D,F,G
            4'd10: seg = 7'b0001000; // A: A,B,C,E,F,G
            4'd11: seg = 7'b0000011; // B: C,D,E,F,G
            4'd12: seg = 7'b1000110; // C: A,D,E,F
            4'd13: seg = 7'b0100001; // D: B,C,D,E,G
            4'd14: seg = 7'b0000110; // E: A,D,E,F,G
            4'd15: seg = 7'b0001110; // F: A,E,F,G
            default: seg = 7'b1111111; // blank display
        endcase
    end

    // Assign output
    assign segment = seg;

endmodule
