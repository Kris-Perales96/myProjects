//The alu will take two source matrices from the execution unit while the read/write signal is low and the enable is low but only on a positive clock edge. Then once the read/write signal goes high 
//it will begin to execute the operation signaled by the op code provided then it will act as a state machine, taking the matrices, rolling the matrices, executing the op code, unrolling then sending out 
//the result onto the bus. 
module ALU(resultMatrix, aluDone, dataBus, matrixIndicator, opCode, readnWrite, nALUenable, nReset, clk);

parameter ROWS = 4;
parameter COLS = 4;
parameter DEPTH = 16;
parameter rollMatrices = 3'd0;
parameter executeOperation = 3'd1;
parameter unrollResult = 3'd2;
parameter sendOutResult = 3'd3;

parameter ADD = 8'd0;
parameter SUBTRACT = 8'd1;
parameter TRANSPOSE = 8'd2;
parameter SCALE = 8'd3;
parameter MULTIPLY = 8'd4;


output reg [255:0] resultMatrix;
inout reg [255:0] dataBus;
input wire [2:0] opCode;
input wire readnWrite, nALUenable, nReset, clk, matrixIndicator;
reg [255:0] matrix1, matrix2;
output reg aluDone;
reg [DEPTH - 1:0] mat1 [ROWS -1:0][COLS - 1:0];
reg [DEPTH - 1:0] mat2 [ROWS -1:0][COLS - 1:0];
reg [DEPTH - 1:0] result [ROWS -1:0][COLS - 1:0];
reg [3:0] aluState;
reg [7:0] scalar;

always @(negedge nReset)
begin
	aluDone = 1'b0;
	scalar = 8'd8;
	resultMatrix = 256'bz;
	matrix1 = 256'bz;
	matrix2 = 256'bz;
	aluState = rollMatrices;
end

always@(posedge clk)
begin
	if(!nALUenable)
		begin
			aluDone = 1'b0;
			if(!readnWrite)
				begin
					if(opCode == ADD || opCode == SUBTRACT || opCode == MULTIPLY)
						begin
							if(matrixIndicator == 0)
								begin
									matrix1 = dataBus;
								end
							else
								begin
									matrix2 = dataBus;
								end
						end
					else if(opCode == SCALE)
						begin
							if(matrixIndicator == 0)
								begin
									matrix1 = dataBus;
								end
							else
								begin
									matrix2 = 256'd0;
									scalar = dataBus;
								end
						end
					else 
						if(matrixIndicator == 0)
								begin
									matrix1 = dataBus;
								end
				end
			
			else
				begin
					case(aluState)
						rollMatrices:
							begin
								
								mat1[0][0] = matrix1[15:0];
								mat1[0][1] = matrix1[31:16];
								mat1[0][2] = matrix1[47:32];
								mat1[0][3] = matrix1[63:48];
								mat1[1][0] = matrix1[79:64];
								mat1[1][1] = matrix1[95:80];
								mat1[1][2] = matrix1[111:96];
								mat1[1][3] = matrix1[127:112];
								mat1[2][0] = matrix1[143:128];
								mat1[2][1] = matrix1[159:144];
								mat1[2][2] = matrix1[175:160];
								mat1[2][3] = matrix1[191:176];
								mat1[3][0] = matrix1[207:192];
								mat1[3][1] = matrix1[223:208];
								mat1[3][2] = matrix1[239:224];
								mat1[3][3] = matrix1[255:240];
								
								
								mat2[0][0] = matrix2[15:0];
								mat2[0][1] = matrix2[31:16];
								mat2[0][2] = matrix2[47:32];
								mat2[0][3] = matrix2[63:48];
								mat2[1][0] = matrix2[79:64];
								mat2[1][1] = matrix2[95:80];
								mat2[1][2] = matrix2[111:96];
								mat2[1][3] = matrix2[127:112];
								mat2[2][0] = matrix2[143:128];
								mat2[2][1] = matrix2[159:144];
								mat2[2][2] = matrix2[175:160];
								mat2[2][3] = matrix2[191:176];
								mat2[3][0] = matrix2[207:192];
								mat2[3][1] = matrix2[223:208];
								mat2[3][2] = matrix2[239:224];
								mat2[3][3] = matrix2[255:240];
							aluState = executeOperation;
							end
							
						executeOperation:
							begin
								case(opCode)
									ADD:
										begin
											$display("Add Result: \n");
											result[0][0] = mat1[0][0] + mat2[0][0];
											result[0][1] = mat1[0][1] + mat2[0][1];
											result[0][2] = mat1[0][2] + mat2[0][2];
											result[0][3] = mat1[0][3] + mat2[0][3];
											result[1][0] = mat1[1][0] + mat2[1][0];
											result[1][1] = mat1[1][1] + mat2[1][1];
											result[1][2] = mat1[1][2] + mat2[1][2];
											result[1][3] = mat1[1][3] + mat2[1][3];
											result[2][0] = mat1[2][0] + mat2[2][0];
											result[2][1] = mat1[2][1] + mat2[2][1];
											result[2][2] = mat1[2][2] + mat2[2][2];
											result[2][3] = mat1[2][3] + mat2[2][3];
											result[3][0] = mat1[3][0] + mat2[3][0];
											result[3][1] = mat1[3][1] + mat2[3][1];
											result[3][2] = mat1[3][2] + mat2[3][2];
											result[3][3] = mat1[3][3] + mat2[3][3];
										end
									SUBTRACT:
										begin
											$display("Subtract Result: \n");
											result[0][0] = mat1[0][0] - mat2[0][0];
											result[0][1] = mat1[0][1] - mat2[0][1];
											result[0][2] = mat1[0][2] - mat2[0][2];
											result[0][3] = mat1[0][3] - mat2[0][3];
											result[1][0] = mat1[1][0] - mat2[1][0];
											result[1][1] = mat1[1][1] - mat2[1][1];
											result[1][2] = mat1[1][2] - mat2[1][2];
											result[1][3] = mat1[1][3] - mat2[1][3];
											result[2][0] = mat1[2][0] - mat2[2][0];
											result[2][1] = mat1[2][1] - mat2[2][1];
											result[2][2] = mat1[2][2] - mat2[2][2];
											result[2][3] = mat1[2][3] - mat2[2][3];
											result[3][0] = mat1[3][0] - mat2[3][0];
											result[3][1] = mat1[3][1] - mat2[3][1];
											result[3][2] = mat1[3][2] - mat2[3][2];
											result[3][3] = mat1[3][3] - mat2[3][3];
										end
									TRANSPOSE:
										begin
											$display("Transpose Result: \n");
											result[0][0] = mat1[0][0];
											result[0][1] = mat1[1][0];
											result[0][2] = mat1[2][0];
											result[0][3] = mat1[3][0];
											result[1][0] = mat1[0][1];
											result[1][1] = mat1[1][1];
											result[1][2] = mat1[2][1];
											result[1][3] = mat1[3][1];
											result[2][0] = mat1[0][2];
											result[2][1] = mat1[1][2];
											result[2][2] = mat1[2][2];
											result[2][3] = mat1[3][2];
											result[3][0] = mat1[0][3];
											result[3][1] = mat1[1][3];
											result[3][2] = mat1[2][3];
											result[3][3] = mat1[3][3];
										end
									SCALE:
										begin
											$display("Scale Result: \n");
											result[0][0] = mat1[0][0] * scalar;
											result[0][1] = mat1[0][1] * scalar;
											result[0][2] = mat1[0][2] * scalar;
											result[0][3] = mat1[0][3] * scalar;
											result[1][0] = mat1[1][0] * scalar;
											result[1][1] = mat1[1][1] * scalar;
											result[1][2] = mat1[1][2] * scalar;
											result[1][3] = mat1[1][3] * scalar;
											result[2][0] = mat1[2][0] * scalar;
											result[2][1] = mat1[2][1] * scalar;
											result[2][2] = mat1[2][2] * scalar;
											result[2][3] = mat1[2][3] * scalar;
											result[3][0] = mat1[3][0] * scalar;
											result[3][1] = mat1[3][1] * scalar;
											result[3][2] = mat1[3][2] * scalar;
											result[3][3] = mat1[3][3] * scalar;
										end
									MULTIPLY:
										begin
											$display("Multiply Result: \n");
											result[0][0] = mat1[0][0]*mat2[0][0] + mat1[0][1]*mat2[1][0] + mat1[0][2]*mat2[2][0]+ mat1[0][3]*mat2[3][0];
											result[0][1] = mat1[0][0]*mat2[0][1] + mat1[0][1]*mat2[1][1] + mat1[0][2]*mat2[2][1]+ mat1[0][3]*mat2[3][1];
											result[0][2] = mat1[0][0]*mat2[0][2] + mat1[0][1]*mat2[1][2] + mat1[0][2]*mat2[2][2]+ mat1[0][3]*mat2[3][2];
											result[0][3] = mat1[0][0]*mat2[0][3] + mat1[0][1]*mat2[1][3] + mat1[0][2]*mat2[2][3]+ mat1[0][3]*mat2[3][3];
											
											result[1][0] = mat1[1][0]*mat2[0][0] + mat1[1][1]*mat2[1][0] + mat1[1][2]*mat2[2][0]+ mat1[1][3]*mat2[3][0];
											result[1][1] = mat1[1][0]*mat2[0][1] + mat1[1][1]*mat2[1][1] + mat1[1][2]*mat2[2][1]+ mat1[1][3]*mat2[3][1];
											result[1][2] = mat1[1][0]*mat2[0][2] + mat1[1][1]*mat2[1][2] + mat1[1][2]*mat2[2][2]+ mat1[1][3]*mat2[3][2];
											result[1][3] = mat1[1][0]*mat2[0][3] + mat1[1][1]*mat2[1][3] + mat1[1][2]*mat2[2][3]+ mat1[1][3]*mat2[3][3];
											
											result[2][0] = mat1[2][0]*mat2[0][0] + mat1[2][1]*mat2[1][0] + mat1[2][2]*mat2[2][0] + mat1[2][3]*mat2[3][0];
											result[2][1] = mat1[2][0]*mat2[0][1] + mat1[2][1]*mat2[1][1] + mat1[2][2]*mat2[2][1] + mat1[2][3]*mat2[3][1];
											result[2][2] = mat1[2][0]*mat2[0][2] + mat1[2][1]*mat2[1][2] + mat1[2][2]*mat2[2][2] + mat1[2][3]*mat2[3][2];
											result[2][3] = mat1[2][0]*mat2[0][3] + mat1[2][1]*mat2[1][3] + mat1[2][2]*mat2[2][3] + mat1[2][3]*mat2[3][3];
											
											result[3][0] = mat1[3][0]*mat2[0][0] + mat1[3][1]*mat2[1][0] + mat1[3][2]*mat2[2][0] + mat1[3][3]*mat2[3][0];
											result[3][1] = mat1[3][0]*mat2[0][1] + mat1[3][1]*mat2[1][1] + mat1[3][2]*mat2[2][1] + mat1[3][3]*mat2[3][1];
											result[3][2] = mat1[3][0]*mat2[0][2] + mat1[3][1]*mat2[1][2] + mat1[3][2]*mat2[2][2] + mat1[3][3]*mat2[3][2];
											result[3][3] = mat1[3][0]*mat2[0][3] + mat1[3][1]*mat2[1][3] + mat1[3][2]*mat2[2][3] + mat1[3][3]*mat2[3][3];
										end
								endcase
							aluState = unrollResult;
							end
						unrollResult:
							begin
								resultMatrix [15:0] = result[0][0];
								resultMatrix [31:16] = result[0][1];
								resultMatrix [47:32] = result[0][2];
								resultMatrix [63:48] = result[0][3];
								resultMatrix [79:64] = result[1][0];
								resultMatrix [95:80] = result[1][1];
								resultMatrix [111:96] = result[1][2];
								resultMatrix [127:112] = result[1][3];
								resultMatrix [143:128] = result[2][0];
								resultMatrix [159:144] = result[2][1];
								resultMatrix [175:160] = result[2][2];
								resultMatrix [191:176] = result[2][3];
								resultMatrix [207:192] = result[3][0];
								resultMatrix [223:208] = result[3][1];
								resultMatrix [239:224] = result[3][2];
								resultMatrix [255:240] = result[3][3];
								aluState = sendOutResult;
							end
						sendOutResult:
							begin
								aluDone = 1'b1;
								aluState = rollMatrices;
								$display("%d %d %d %d \n", resultMatrix[255:240], resultMatrix [239:224], resultMatrix[223:208], resultMatrix[207:192]);
								$display("%d %d %d %d \n", resultMatrix[191:176], resultMatrix [175:160], resultMatrix[159:144], resultMatrix[143:128]);
								$display("%d %d %d %d \n", resultMatrix[127:112], resultMatrix [111:96], resultMatrix [95:80], resultMatrix [79:64]);
								$display("%d %d %d %d \n", resultMatrix[63:48], resultMatrix [47:32], resultMatrix [31:16], resultMatrix [15:0]);
								
								
							end
					endcase
				end
		end
end

assign dataBus = (!nALUenable && readnWrite) ? resultMatrix : 256'bz;

endmodule
