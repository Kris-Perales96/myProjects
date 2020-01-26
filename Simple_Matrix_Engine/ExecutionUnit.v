//The execution unit will serve as the control of the simplistic processor we are creating to perform matrix operations. 

//Thing to note from the presentation is that now the scalar is pulled off of the instruction
module ExecutionUnit(nReset, clk);

input wire nReset, clk;
reg [7:0] src1, src2, dest;
reg [2:0] aluOPcode;
wire [2:0] instrNumber;
reg [31:0] currentInstruction;
reg countSig, driveBus;
reg [2:0] offset;
wire [31:0] instrMemLine;
wire [255:0] dataBus;
reg [7:0] address;
reg nMatrixMemEnable, ReadnWriteMem, nIMemEnable,readnWriteALU, nALUenable, matrixIndicator;
reg [7:0] currentStage;
wire [255:0] resultMatrix;
wire aluDone;

parameter SCALE = 8'd3;

parameter fetchInstruction = 8'd0;
parameter transferOfInstruction = 8'd1;
parameter instructionRetrieved = 8'd2;

parameter instructionDecode = 8'd10;
parameter retrieveSource1 = 8'd11;
parameter transferSource1toALU = 8'd12;
parameter bufferForSource1 = 8'd13;
parameter retrieveSource2 = 8'd14;
parameter transferSource2toALU = 8'd15;
parameter bufferForSource2 = 8'd16;
parameter loadedMatrices = 8'd17;

parameter waitForALU = 8'd20;
parameter setUpSignals = 8'd21;

parameter writeToDestination = 8'd30;
parameter bufferForDestination = 8'd31;

parameter cleanUpSignalsAndRegs = 8'd40; 


matrixMemoryRevised matrixMem1(dataBus, address, nMatrixMemEnable, ReadnWriteMem, clk, nReset);
IMem instrMem1(instrMemLine, clk, instrNumber, nReset, nIMemEnable);
ALU alu1(resultMatrix, aluDone, dataBus, matrixIndicator, aluOPcode, readnWriteALU, nALUenable, nReset, clk);
Counter_3Bit programCounter(instrNumber, countSig, nReset, offset);

always @(negedge nReset)
begin
	address = 8'd0;
	nMatrixMemEnable = 1'b1;
	nIMemEnable = 1'b1;
	currentStage = fetchInstruction;
	aluOPcode = 3'bz;
	src1 = 8'bz;
	src2 = 8'bz;
	dest = 8'bz;
	currentInstruction = 32'bz;
	ReadnWriteMem = 1'b1;
	readnWriteALU = 1'b0;
	nALUenable = 1'b1;
	matrixIndicator = 1'bz;
	countSig = 1'b0;
	offset = 8'd1;
	driveBus = 1'b0;
end

always @(posedge clk)
begin
	case(currentStage)
		fetchInstruction:
			begin
				countSig = 1'b0;
				nIMemEnable = 1'b0;
				currentStage = transferOfInstruction;
			end
		transferOfInstruction:
			begin
				currentInstruction = instrMemLine;
				currentStage = instructionDecode;
			end
		instructionDecode:
			begin
				if(currentInstruction == 32'hffffffff)
					begin
						$stop;
					end
				aluOPcode = instrMemLine[26:24];
				dest = instrMemLine[23:16];
				src1 = instrMemLine[15:8];
				src2 = instrMemLine[7:0];
				currentStage = retrieveSource1;
				nMatrixMemEnable = 1'b0;
				nIMemEnable = 1'b1;
				readnWriteALU = 1'b0;
				
			end
		retrieveSource1:
			begin
				address = src1;
				currentStage = transferSource1toALU;
				nALUenable = 1'b0;
				matrixIndicator = 1'b0;
			end
		transferSource1toALU:
			begin
				currentStage = retrieveSource2;
			end
		retrieveSource2:
			begin
				if(src2 == 8'hff)
					begin
						currentStage = loadedMatrices;
					end
				if(aluOPcode ==  SCALE)
					begin
						driveBus = 1'b1;
						matrixIndicator = 1'b1;
						nMatrixMemEnable = 1'b1;
						currentStage = transferSource2toALU;
					end
				address = src2;
				matrixIndicator = 1'b1;
				currentStage = transferSource2toALU;
			end
		transferSource2toALU:
			begin
				nALUenable = 1'b0;
				currentStage = loadedMatrices;
			end
		loadedMatrices:
			begin
				driveBus = 1'b0;
				readnWriteALU = 1'b1;
				nMatrixMemEnable = 1'b1;
				currentStage = waitForALU;
			end
		waitForALU:
			begin
				if(!aluDone)
					begin
						currentStage = waitForALU;
					end
				else
					begin
						currentStage = writeToDestination;
					end
			end
		writeToDestination:
			begin
				ReadnWriteMem = 1'b0;
				address = dest;
				currentStage = bufferForDestination;
			end
		bufferForDestination:
			begin
				nMatrixMemEnable = 1'b0;
				currentStage = cleanUpSignalsAndRegs;
			end
		cleanUpSignalsAndRegs:
			begin
				countSig =1'b1;
				address = 4'd0;
				nMatrixMemEnable = 1'b1;
				nIMemEnable = 1'b1;
				currentStage = fetchInstruction;
				aluOPcode = 3'bz;
				src1 = 8'bz;
				src2 = 8'bz;
				dest = 8'bz;
				currentInstruction = 32'bz;
				ReadnWriteMem = 1'b1;
				readnWriteALU = 1'b0;
				nALUenable = 1'b1;
				matrixIndicator = 1'bz;
				driveBus = 1'b0;
			end
		endcase
end

assign dataBus = (driveBus) ? src2 : 256'bz;

endmodule
