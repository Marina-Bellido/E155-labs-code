//------------------------------------------------------------------------------
// Author:   Marina Bellido (mbellido@hmc.edu)
// Date:     28/8/2025
// File:     sevsegtestbench.sv
//------------------------------------------------------------------------------
// Description:
//   Testbench for the 7-segment display module `seven_segment`.
//   - Applies test vectors from a file to the input `s`.
//   - Compares the module output `segment` to the expected values.
//   - Reports mismatches and counts errors.
//------------------------------------------------------------------------------

module sevsegtestbench();
    // Testbench signals
    logic clk, reset;
    logic [3:0] s;                    // Input to DUT
    logic [6:0] segment, segment_expected; // 7-segment outputs
    logic [31:0] vectornum, errors;   // Counters for test vectors and errors
    logic [10:0] testvectors[10000:0]; // Test vector memory

    // Instantiate device under test (DUT)
    seven_segment dut(s, seg);
    
    // Clock generation: 10 ns period
    always begin
        clk = 1; #5; clk = 0; #5;
    end
    
    // Load test vectors and initialize signals
    initial begin
        $readmemb("sevegtest.tv", testvectors); // Load test vectors from file
        vectornum = 0; 
        errors = 0;
        reset = 1; #22; reset = 0; // Apply initial reset
    end
    
    // Apply test vectors at rising edge of clock
    always @(posedge clk) begin
        #1; // Small delay to allow signal propagation
        {s, segment_expected} = testvectors[vectornum];
    end
    
    // Check DUT outputs on falling edge of clock
    always @(negedge clk)
    if (~reset) begin
        // Compare DUT output to expected segment pattern
        if (segment !== segment_expected) begin
            $display("Error: inputs = %b", num);
            $display(" outputs = %b (%b expected)", segment, segment_expected);
            errors = errors + 1;
        end
        vectornum = vectornum + 1;
        
        // Stop simulation when all test vectors are applied
        if (testvectors[vectornum] === 11'bx) begin
            $display("%d tests completed with %d errors", vectornum, errors);
            $stop;
        end
    end
endmodule

