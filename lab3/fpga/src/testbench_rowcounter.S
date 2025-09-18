//=============================================================
// Testbench: testbench_rowcounter
// Author: Marina Bellido 
// Date: 18/9/2025
//
// This testbench verifies the functionality of the row_counter
// module. It drives column inputs in sequence, generates a 
// simulated clock and reset, and compares DUT outputs 
// (rows, key_val, pushed) against expected values using 
// assertions. It models the row scan pattern with a circular 
// shift and computes expected key values based on active rows 
// and columns.
//=============================================================

// Modelsim-ASE requires a timescale directive
`timescale 1ns / 1ns

module testbench_rowcounter();
    // Testbench signals
    logic reset, clk;
    logic [7:0] key_val, key_val_expected;
    logic       pushed, pushed_expected;
    logic [3:0] rows, rows_expected; 
    //logic [3:0] col_options; // optional: all possible column combinations
    logic [3:0] row_pattern, cols;
    logic stop_counter, write_enable;
	
    // Instantiate device under test (DUT)
    row_counter dut(reset, clk, stop_counter, write_enable, cols, rows, key_val, pushed);

    // Generate 50 MHz clock (20 ns period)
    always begin
        clk = 1; #10; 
        clk = 0; #10;
    end
	
    // Reset sequence
    initial begin
        reset = 0; 
        #50
        reset = 1;
    end
	
    // Drive column inputs and control signals over time
    initial begin
        cols = 4'b0000;
        stop_counter = 0;
        write_enable = 0;
		
        #200
        cols = 4'b0001; // Press column 0
        stop_counter = 1;
        write_enable = 1;
		
        #200
        cols = 4'b0000; // Release column
        stop_counter = 0;
        write_enable = 0;
		
        #200
        cols = 4'b0010; // Press column 1
        stop_counter = 1;
        write_enable = 1;
		
        #200
        cols = 4'b0000; // Release column
        stop_counter = 0;
        write_enable = 0;
		
        #200
        cols = 4'b0100; // Press column 2
        stop_counter = 1;
        write_enable = 1;
		
        #200
        cols = 4'b0000; // Release column
        stop_counter = 0;
        write_enable = 0;
		
        #200
        cols = 4'b1000; // Press column 3
        stop_counter = 1;
        write_enable = 1;
    end

    // Expected row counter logic (circular shift)
    always_ff @(posedge clk, negedge reset) begin
        if(!reset) begin
            rows_expected <= 4'b0001;
            row_pattern   <= 4'b0001;
        end else if (!stop_counter) begin  
            row_pattern   <= {row_pattern[2:0], row_pattern[3]}; // circular left shift
            rows_expected <= row_pattern;
        end else begin
            rows_expected <= row_pattern;
        end
		 	
        // Assertions to check DUT vs expected behavior
        //assert (rows == rows_expected) else $error("Assertion failed row: %b %b", rows, rows_expected);
        assert (key_val == key_val_expected) else $error("Assertion failed key_val: %b %b", key_val, key_val_expected);
        //assert (pushed == pushed_expected) else $error("Assertion failed push: %b %b", pushed, pushed_expected);
    end
		
    // Placeholder for any additional sequential checks
    always_ff @(posedge clk) begin
    end

    // Combinational logic for expected push/key values
    always_comb begin
        pushed_expected = $onehot(cols); // true only if one column is active
        if(write_enable) begin
            // Lookup table: map row/col combination to key value
            case({rows_expected[1], rows_expected[2], cols[0], rows_expected[3], cols[1], cols[2], cols[3], rows_expected[0]})
                8'b0001_1000: key_val_expected = 4'h0;  // 0 = R3 & C1 
                8'b0010_0001: key_val_expected = 4'h1;  // 1 = R0 & C0
                8'b0000_1001: key_val_expected = 4'h2;  // 2 = R0 & C1
                8'b0000_0101: key_val_expected = 4'h3;  // 3 = R0 & C2
                8'b1010_0000: key_val_expected = 4'h4;  // 4 = R1 & C0
                8'b1000_1000: key_val_expected = 4'h5;  // 5 = R1 & C1
                8'b1000_0100: key_val_expected = 4'h6;  // 6 = R1 & C2
                8'b0110_0000: key_val_expected = 4'h7;  // 7 = R2 & C0
                8'b0100_1000: key_val_expected = 4'h8;  // 8 = R2 & C1
                8'b0100_0100: key_val_expected = 4'h9;  // 9 = R2 & C2
                8'b0000_0011: key_val_expected = 4'hA;  // A = R0 & C3 
                8'b1000_0010: key_val_expected = 4'hB;  // B = R1 & C3
                8'b0100_0010: key_val_expected = 4'hC;  // C = R2 & C3 
                8'b0001_0010: key_val_expected = 4'hD;  // D = R3 & C3 
                8'b0011_0000: key_val_expected = 4'hE;  // E = R3 & C0 
                8'b0001_0100: key_val_expected = 4'hF;  // F = R3 & C2 
                default:       key_val_expected = 4'h0; 
            endcase
        end
        // Assertions to cross-check rows and push signal
        assert (rows == rows_expected) else $error("Assertion failed row: %b %b", rows, rows_expected);
        assert (pushed == pushed_expected) else $error("Assertion failed push: %b %b", pushed, pushed_expected);
    end
	
endmodule
