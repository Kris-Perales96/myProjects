module DFF1Project(D, clk, reset, Q);

input wire D;
input wire reset, clk;
output reg Q;

always @(posedge clk)
begin
	if (~reset)
		begin
			Q <= 1'b0;
		end
	else
		begin
			Q <= D;
		end
end
endmodule
