module AccumulatorOne(outD, muxOut, reset, clk);
output reg outD;
input wire reset;
input wire clk;
input wire muxOut;

always @(posedge clk) begin
	if(reset)
	begin
		outD <= 1'b0;
	end
	else
	begin
		outD <= muxOut;
	end
		
end

endmodule
