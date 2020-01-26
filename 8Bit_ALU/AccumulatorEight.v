module AccumulatorEight(outD, muxOut, reset, clk);
output reg [7:0] outD;
input reset;
input clk;
input [7:0] muxOut;

always @(posedge clk) begin
	if(reset)
	begin
		outD = 8'd0;
	end
	else
	begin
		outD = muxOut;
	end
		
end

endmodule
