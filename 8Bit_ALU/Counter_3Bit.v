module Counter_3Bit(instrOut, count, reset, offset);
input wire count, reset; //Reset is an active low input
parameter width = 4; //enables the ability to have any bit counter
input wire [(width-1):0] offset; // enables to have an arbitrary offset
output reg [(width-1):0] instrOut;

//Used the code provided in class to construct the counter

always @ (posedge count or negedge reset)
begin
if(!reset)
	instrOut = 4'd0;
else
	instrOut = instrOut + offset;
end

endmodule
