// Modelsim-ASE requires a timescale directive
`timescale 1 ns / 1 ns

module testbenchlab1();
    // Testbench signals
    logic clk, reset;
    logic [3:0] s;                    // Input to DUT
    logic [6:0] segment, segment_expected; // 7-segment outputs
    logic [2:0] led;
	logic [1:0] led_expected;         // Expected LED outputs
    logic [31:0] vectornum, errors;   // Counters
    logic [12:0] testvectors[20:0];   // Test vector memory

    // Instantiate device under test (DUT)
    lab11_mb dut(clk, s, led, segment);
    
    // Clock generation: 10 ns period (100 MHz)
    always begin
        clk = 1; #5; clk = 0; #5;
    end
    
    // Initialize test vectors and reset
    initial begin
        $readmemb("testvector1.tv", testvectors); // Load test vectors from file
        vectornum = 0; 
        errors = 0;
        reset = 1; #22; reset = 0; // Apply initial reset
    end
    
    // Apply test vectors at rising edge of clock
    always @(posedge clk) begin
        #1; // small delay to allow propagation
        {s, led_expected[1:0], segment_expected} = testvectors[vectornum];
    end
    
    // Check DUT outputs on falling edge
    always @(negedge clk)
    if (~reset) begin
        // Compare DUT outputs to expected values
        if (led[1:0] !== led_expected[1:0] || segment !== segment_expected) begin
            $display("Error: inputs = %b", s);
            $display(" outputs = %b %b (%b %b expected)", led[1:0], segment, led_expected[1:0], segment_expected);
            errors = errors + 1;
        end
        vectornum = vectornum + 1;
        
        // Stop simulation when all test vectors are done
        if (testvectors[vectornum] === 13'bx) begin
            $display("%d tests completed with %d errors", vectornum, errors);
            $stop;
        end
    end
endmodule
