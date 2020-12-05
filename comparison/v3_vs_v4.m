%v3 single thread operation times
v3_bg = 0.023;
v3_yt = 9.5;
v3_d10 = 0.13;
v3_m13 = 3.5;
v3_nc = 0.31;


% v4 single thread operation times
v4_bg = 0.044;
v4_yt = 5.49;
v4_d10 = 0.14;
v4_m13 = 1.71;
v4_nc = 0.5;



figure;
X = categorical({'com-Youtube', 'mycielskian13'});
X = reordercats(X,{'com-Youtube', 'mycielskian13'});
bar(X, [v3_yt v4_yt; v3_m13 v4_m13])
title('Single Thread Performance v3 vs v4');
ylabel('Operation Time (s)');
legend('v3', 'v4');

figure;
X = categorical({'dblp-2010', 'NACA0015', 'belgium-osm'});
X = reordercats(X,{'dblp-2010', 'NACA0015', 'belgium-osm'});
bar(X, [v3_d10 v4_d10; v3_nc v4_nc ; v3_bg v4_bg])
title('Single Thread Performance v3 vs v4');
ylabel('Operation Time (s)');
legend('v3', 'v4');

