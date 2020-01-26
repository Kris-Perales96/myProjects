module RCA8Project(a,b,cin, cout, sum, OF);
input [7:0] a, b;
input cin;
output [7:0] sum;
output OF, cout;
wire [7:1] w;
wire [7:0] k;

xor xor0(k[0], b[0], cin);
xor xor1(k[1], b[1], cin);
xor xor2(k[2], b[2], cin);
xor xor3(k[3], b[3], cin);
xor xor4(k[4], b[4], cin);
xor xor5(k[5], b[5], cin);
xor xor6(k[6], b[6], cin);
xor xor7(k[7], b[7], cin);

FAProject FA0(a[0], k[0], cin, w[1], sum[0]);
FAProject FA1(a[1], k[1], w[1], w[2], sum[1]);
FAProject FA2(a[2], k[2], w[2], w[3], sum[2]);
FAProject FA3(a[3], k[3], w[3], w[4], sum[3]);
FAProject FA4(a[4], k[4], w[4], w[5], sum[4]);
FAProject FA5(a[5], k[5], w[5], w[6], sum[5]);
FAProject FA6(a[6], k[6], w[6], w[7], sum[6]);
FAProject FA7(a[7], k[7], w[7], cout, sum[7]);

xor xor8(OF, cout, w[7]);

endmodule
