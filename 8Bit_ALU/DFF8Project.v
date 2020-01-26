module DFF8Project(D, clk, reset, Q);

input wire [7:0] D;
input wire reset, clk;
output reg [7:0] Q;

always @(posedge clk)
begin
	if (~reset)
		begin
			Q <= 8'b00000000;
		end
	else
		begin
			Q <= D;
		end
end

endmodule
