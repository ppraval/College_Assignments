module FirstPain_test;
    reg a, b;
    wire y;

    and_gate uut (a,b,s);

    initial
		begin
        $dumpfile("and_gate_tb.vcd");
        $dumpvars(0, and_gate_tb);

        a = 0; b = 0; #10;
        a = 0; b = 1; #10;
        a = 1; b = 0; #10;
        a = 1; b = 1; #10;

        $finish;
    end
endmodule
