module helloworldfpga(
    input BUTTON,
    output wire LED
);
    wire clk;

    qlal4s3b_cell_macro u_qlal4s3b_cell_macro (
        .Sys_Clk0 (clk),
    );

    reg	[23:0]	cnt;
    reg	[23:0]	stopcnt;
    reg		led;
    initial cnt <= 0;
    initial stopcnt <= 4000000;		// Change to 2000000
    initial led <= 0;

    always @(posedge clk) begin
	if (cnt == stopcnt) begin
		cnt <= 0;
		led <= ~led;
	end else begin
        cnt <= cnt + 1;
	end
    end
    assign LED = led & BUTTON;	// Change to redled = led;
endmodule