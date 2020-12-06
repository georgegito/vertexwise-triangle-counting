% speedup function

function res = speedup(op_times, num_of_threads)

    n = length(num_of_threads);
    sp = [];
    
    for i = 1:n
        spi = op_times(1)/op_times(i);
        sp = [sp spi];
    end

    plot(num_of_threads, sp, '-o', 'linewidth', 2);
    grid on
    
    res = sp;

end


