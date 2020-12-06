% create speedup plots

data;

figure;

speedup(v3_cilk_bg, num_of_threads);
hold on;
speedup(v3_cilk_yt, num_of_threads);
hold on;
speedup(v3_cilk_d10, num_of_threads);
hold on;
speedup(v3_cilk_m13, num_of_threads);
hold on;
speedup(v3_cilk_nc, num_of_threads);

title('v3 Cilk Speedup');
xlabel('Number of Threads', 'fontweight', 'bold');
ylabel('Speedup', 'fontweight', 'bold');
legend('belgium-osm', 'com-Youtube', 'dblp-10', 'mycielskian13', 'NACA0015');
xticks([1, 2, 8, 16, 20]);
yticks(1:9);
hold off;



figure;

speedup(v3_omp_bg, num_of_threads);
hold on;
speedup(v3_omp_yt, num_of_threads);
hold on;
speedup(v3_omp_d10, num_of_threads);
hold on;
speedup(v3_omp_m13, num_of_threads);
hold on;
speedup(v3_omp_nc, num_of_threads);

title('v3 OpenMP Speedup');
xlabel('Number of Threads', 'fontweight', 'bold');
ylabel('Speedup', 'fontweight', 'bold');
legend('belgium-osm', 'com-Youtube', 'dblp-10', 'mycielskian13', 'NACA0015');
xticks([1, 2, 8, 16, 20]);
yticks(1:14);
hold off;



figure;

speedup(v4_cilk_bg, num_of_threads);
hold on;
speedup(v4_cilk_yt, num_of_threads);
hold on;
speedup(v4_cilk_d10, num_of_threads);
hold on;
speedup(v4_cilk_m13, num_of_threads);
hold on;
speedup(v4_cilk_nc, num_of_threads);

title('v4 Cilk Speedup');
xlabel('Number of Threads', 'fontweight', 'bold');
ylabel('Speedup', 'fontweight', 'bold');
legend('belgium-osm', 'com-Youtube', 'dblp-10', 'mycielskian13', 'NACA0015');
xticks([1, 2, 8, 16, 20]);
yticks(1:11);
hold off;



figure;

speedup(v4_omp_bg, num_of_threads);
hold on;
speedup(v4_omp_yt, num_of_threads);
hold on;
speedup(v4_omp_d10, num_of_threads);
hold on;
speedup(v4_omp_m13, num_of_threads);
hold on;
speedup(v4_omp_nc, num_of_threads);

title('v4 OpenMP Speedup');
xlabel('Number of Threads', 'fontweight', 'bold');
ylabel('Speedup', 'fontweight', 'bold');
legend('belgium-osm', 'com-Youtube', 'dblp-10', 'mycielskian13', 'NACA0015');
xticks([1, 2, 8, 16, 20]);
yticks(1:12);
hold off;



figure;

speedup(v4_pt_bg, num_of_threads);
hold on;
speedup(v4_pt_yt, num_of_threads);
hold on;
speedup(v4_pt_d10, num_of_threads);
hold on;
speedup(v4_pt_m13, num_of_threads);
hold on;
speedup(v4_pt_nc, num_of_threads);

title('v4 PThreads Speedup');
xlabel('Number of Threads', 'fontweight', 'bold');
ylabel('Speedup', 'fontweight', 'bold');
legend('belgium-osm', 'com-Youtube', 'dblp-10', 'mycielskian13', 'NACA0015');
xticks([1, 2, 8, 16, 20]);
yticks(1:12);
hold off;




    	