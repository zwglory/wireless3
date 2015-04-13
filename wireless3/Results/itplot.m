itload('sc_result_file.it');
figure(1); clf;
h1 = semilogy(EbN0dB,ber,'*-r'); hold on
ebn0db = 10:.1:25;
ebn0 = 10.^(ebn0db/10);
% Pb = 0.5 * erfc(sqrt(ebn0));
% h2 = semilogy(ebn0db,Pb);
set(gca,'fontname','times','fontsize',10);
xlabel('{\it E_b} / {\it N}_0 [dB]');
ylabel('BER')
title('Spinal code on an AWGN Channel');
axis([-1, inf, -0.5, 1]);
legend(h1,'Simulation');
grid on;
