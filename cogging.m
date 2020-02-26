%%
data = readtable('tmp.csv');
%%
np = 20; % pole pairs
n = 8192;%1024;
l = 20000;

t = unwrap((data.mcu_timestamp0-data.mcu_timestamp0(1))*2*pi/2^32)*2^32/2/pi/170e6;
pos = data.motor_position0;
enc = data.motor_encoder0;
iqraw = data.iq0;
inds_trim = pos < .95*max(pos) & pos > .95*min(pos);
x = mod(enc(inds_trim),l);
iq = data.iq0(inds_trim);
figure(1); hold on
plot(x, iq,'.')
xlabel('encoder position (counts)')
ylabel('current measured (A)')

% mean of values in n bins
xr = linspace(0,l, n+1);
[~,i] = histc(x, xr);
i = accumarray(i,iq, [], @mean);

plot(xr(1:end-1), i)

f = fft(i);
% get 20 largest values
[~,ind] = maxk(abs(f), 20); % or should it be limited in frequency?
ind = abs(freq) < np*20;    % frequency limit
% filtered current
mf = zeros(size(f));
mf(ind) = f(ind);
mf(1) = 0; % remove average
iq_filt = ifft(mf);

plot(xr(1:end-1), iq_filt)
fid = fopen('motorlib/cogprocessed.csv','w');
fprintf(fid,'%f,',iq_filt);
fclose(fid);
legend('raw values','averaged', 'filtered')

freq = fftfreq(length(f),1/n);
figure(2);
semilogy(freq,abs(f))
hold on
xlabel('frequency ()')
ylabel('magnitude')

%%
figure(3);
plot(t, iqraw)

figure(4);
plot(t, unwrap(enc*2*pi/l)*l/2/pi+l, t(inds_trim), unwrap(x*2*pi/l)*l/2/pi)
