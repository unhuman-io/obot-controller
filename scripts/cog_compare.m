%%
cog_files = dir('cog*');
figure(1); clf;
for i = 1:length(cog_files)
    data = readmatrix(cog_files(i).name)';
    plot(linspace(0,1-1/length(data),length(data))', data);
    hold on;
end