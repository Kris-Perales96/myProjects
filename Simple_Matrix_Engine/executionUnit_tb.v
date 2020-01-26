module executionUnit_tb;
reg clk, nReset;

ExecutionUnit exe1(nReset, clk);

initial
begin
	nReset = 1'b0;
	#2 nReset = 1'b1;
end

initial
begin
	clk = 1'b0;
	forever #5 clk = ~clk;
end

endmodule
