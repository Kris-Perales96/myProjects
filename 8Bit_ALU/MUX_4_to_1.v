module MUX_4_to_1(muxOut, sel, sig1, sig2, sig3, sig4);
input wire [1:0] sel;
input wire sig1, sig2, sig3, sig4;
output muxOut;

wire selN, selN1;
wire [3:0] w;

not(selN, sel[0]);
not(selN1, sel[1]);

and and0(w[0], selN, selN1, sig1);
and and1(w[1], sel[0], selN1, sig2);
and and2(w[2], selN, sel[1], sig3);
and and3(w[3], sel[0], sel[1], sig4);

or or1(muxOut, w[0], w[1], w[2], w[3]);

endmodule
