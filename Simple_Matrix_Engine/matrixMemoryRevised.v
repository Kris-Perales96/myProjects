//The following module is the memory for the matrices used in the project as well as (for the moment) the result register for the transpose matrix, time permitting will be moved to its own memory 
//where the two memories will share the same 256 bit busline. The module has three signals from the execution unit: reset, enable and read/write signal where the reset, enable, and write are active low signals.
//While the memory is not enabled it will act as high Z, but while it is enabled it will read from the module so long as the read/write signal is high, then write to the given address if the 
//read/write signal is low. The address may only need to be 3 bits total since the memory is only 7 deep, it is 8 bits since the opcode source and destination are 8 bits each.  
module matrixMemoryRevised(dataBus, address, nMatrixMemEnable, ReadnWrite, clk, nReset);
input wire ReadnWrite, clk, nReset, nMatrixMemEnable;
input wire [7:0] address;
inout reg [255:0] dataBus;
reg [255:0] matrixMemory[11:0];
reg [255:0] matrixOut;
reg [255:0] resultRegister;

always @(negedge nReset)
begin
	matrixOut = 256'bz;
	matrixMemory[0] = 256'h0004_000c_0004_0022_0007_0006_000b_0009_0009_0002_0008_000d_0002_000f_0010_0003;
	matrixMemory[1] = 256'h0017_002d_0043_0016_0007_0006_0004_0001_0012_0038_000d_000c_0003_0005_0007_0009;
	matrixMemory[2] = 256'd0;
	matrixMemory[3] = 256'd0;
	matrixMemory[4] = 256'd0;
	matrixMemory[5] = 256'd0;
	matrixMemory[6] = 256'd0;
	matrixMemory[7] = 256'd0;
	matrixMemory[8] = 256'd0;
end 

always@(posedge clk)
begin
	if(!nMatrixMemEnable)
		begin
			if(address <= 4'd9)
				begin
					if(!ReadnWrite)
						begin
							matrixMemory[address] = dataBus;
						end
					else
						begin
							matrixOut = matrixMemory[address];
						end
				end
			else
				begin
					if(!ReadnWrite)
						begin
							resultRegister = dataBus;
						end
					else
						begin
							matrixOut = resultRegister;
						end
				end
		end
end

assign dataBus = (!nMatrixMemEnable && ReadnWrite) ? matrixOut : 256'bz;

endmodule
