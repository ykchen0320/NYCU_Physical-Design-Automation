.SUBCKT ASYNC_DFFHx1_ASAP7_75t_R CLK D QN RESET SET VDD VSS
MM43 pd3 SET VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM24 QN SH VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM29 SS SH VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM23 clkb clkn VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM20 clkn CLK VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM44 SS RESET VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM34 SH clkn pd3 VSS nmos_rvt w=81.0n l=20n nfin=3
MM48 net020 RESET VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM12 MS clkb SH VSS nmos_rvt w=81.0n l=20n nfin=3
MM33 pd3 SS VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM9 MH clkb net020 VSS nmos_rvt w=81.0n l=20n nfin=3
MM8 net020 MS VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM6 MS MH VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM5 pd1 D VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM4 MH clkn pd1 VSS nmos_rvt w=81.0n l=20n nfin=3
MM47 MS SET VSS VSS nmos_rvt w=81.0n l=20n nfin=3
MM25 QN SH VDD VDD pmos_rvt w=162.0n l=20n nfin=6
MM28 SS SH net076 VDD pmos_rvt w=162.0n l=20n nfin=6
MM22 clkb clkn VDD VDD pmos_rvt w=162.0n l=20n nfin=6
MM45 net076 RESET VDD VDD pmos_rvt w=162.0n l=20n nfin=6
MM46 net079 SET VDD VDD pmos_rvt w=162.0n l=20n nfin=6
MM21 clkn CLK VDD VDD pmos_rvt w=162.0n l=20n nfin=6
MM37 pd2 SS net077 VDD pmos_rvt w=162.0n l=20n nfin=6
MM35 SH clkb pd2 VDD pmos_rvt w=162.0n l=20n nfin=6
MM49 net078 RESET VDD VDD pmos_rvt w=162.0n l=20n nfin=6
MM13 MS clkn SH VDD pmos_rvt w=162.0n l=20n nfin=6
MM11 net051 MS net078 VDD pmos_rvt w=162.0n l=20n nfin=6
MM10 MH clkn net051 VDD pmos_rvt w=162.0n l=20n nfin=6
MM7 MS MH net079 VDD pmos_rvt w=162.0n l=20n nfin=6
MM42 net077 SET VDD VDD pmos_rvt w=162.0n l=20n nfin=6
MM1 MH clkb pu1 VDD pmos_rvt w=162.0n l=20n nfin=6
MM3 pu1 D VDD VDD pmos_rvt w=162.0n l=20n nfin=6
.ENDS