module IMem(instrMemLine, clk, instrNumber, nReset, nIMemEnable);
//The reset and instruction memory are active low because it is easier to ground a device, as well as avoid the potential for the transistors to be in no man's land.
//For the moment the instruction memory has its own busline of 32-bits to develop the simplest foundation that can be extended later to share the 256 bit busline from
//Main Memory and single register for the matrcies. Which is why the instruction memory still has an enable signal and why it will be high Z when the memory module is not enabled.
//The instruction memory will send out the corresponding instruction to the inputted instruction number onto the 32 bit busline to the execution unit, but if time allows will be expanded 
//to share the 256 busline from the matrix memory and result register.
input wire clk, nReset, nIMemEnable;
input wire [2:0] instrNumber;
reg [31:0] instructionMem [9:0];
output reg [31:0] instrMemLine;

//The instructions are structured as follows with each part being 8 bits: opcode, destination, source1, source2. Where at the moment the single result register will be put in the matrix memory module 
//32'h00_02_00_01: add, 2, 0, 1 this instruction signifies an add instruction of the memory stored in mainMemory[0] mainMemory[1] with the result being stored in mainMemory[2]
//32'h01_03_02_00: subtract 3, 2, 0 subtract mainMemory[0] from mainMemory[2] and store result in mainMemory[3]
//32'h02_04_02_09: transpose 4, 2, ff transpose of mainMemory[2] store into mainMemory[4] ff signifies nothing should be put out onto the bus for source2
//32'h03_06_04_08: scale, 10, 4, 8 multiply mainMemory[4] by the immediate stored in the ALU of decimal 8 and store into the result register, any address larger than 8, located in mainMemory
//32'h04_05_06_01: multiply 5, 10, 4 multiply the result register, any address larger than 8, by mainMemory[4] and store the result in mainMemory[5]
//32'hff_ff_ff_ff: stop opcode to tell the execution unit it has reached the end on iMem sequence

always @(negedge nReset)
	begin
		instrMemLine = 32'bz;
		instructionMem[0] = 32'h00_02_00_01;
		instructionMem[1] = 32'h01_03_02_00;
		instructionMem[2] = 32'h02_04_02_ff;
		instructionMem[3] = 32'h03_0b_04_08;
		instructionMem[4] = 32'h04_05_0b_04;
		instructionMem[5] = 32'hff_ff_ff_ff;
	end
always @(posedge clk)
begin
	if(!nIMemEnable)
		instrMemLine = instructionMem[instrNumber];
end


endmodule
