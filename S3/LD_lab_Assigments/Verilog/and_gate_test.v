module and_gate_test;
    reg a, b;
    wire x;

    and_gate uut (a, b, x);

    initial begin
        $dumpfile("and_gate_test.vcd");
        $dumpvars(0, and_gate_test);

        a = 0; b = 0; #10;
        a = 0; b = 1; #10;
        a = 1; b = 0; #10;
        a = 1; b = 1; #10;
    end
endmodule
