//=============================================================
// Testbench: testbench_bouncer
// 
// This testbench verifies the debouncer module. It generates 
// a clock and reset, drives the debounce_enable signal, and 
// checks that the DUT output (time_passed) matches the expected 
// timing behavior. The expected counter is modeled in parallel 
// with assertions used to compare DUT vs. expected output.
//=============================================================

// Modelsim-ASE requires a timescale directive
`timescale 1ns / 1ns

module testbench_bouncer();
    // Testbench signals
    logic reset, clk, debounce_enable;
    logic time_passed, time_passed_expected;
    logic [23:0] counting; // reference counter for expected behavior
  
    // Instantiate device under test (DUT)
    debouncer dut(reset, clk, debounce_enable, time_passed);

    // Generate 50 MHz clock (20 ns period)
    always begin
        clk = 1; #10; 
        clk = 0; #10;
    end
	
    // Reset sequence
    initial begin
        reset = 0; 
        #22 
        reset = 1;
    end

    // Stimulus: toggle debounce_enable
    initial begin     
        debounce_enable = 0;
        #500;
        debounce_enable = 1;
        #5000;
    end
	
    // Expected behavior model + assertions
    always_ff @(posedge clk, negedge reset) begin
        if(!reset) begin
            counting <= 0;
            time_passed_expected <= 0;
        end else if (debounce_enable) begin    
            if(counting == 23'd500) begin  // example threshold (smaller than DUT)
                counting <= 0;
                time_passed_expected <= ~time_passed_expected; // toggle expected output
            end else begin 
                counting <= counting + 1;
                time_passed_expected <= 0;
            end
        end else begin 
            counting <= 0;
            time_passed_expected <= 0;
        end
		 
        // Assertion: compare DUT output against expected output
        assert (time_passed == time_passed_expected) 
            else $error("Assertion failed time_passed: %b %b", 
                        time_passed, time_passed_expected);
    end 
endmodule
