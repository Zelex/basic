(println "Number of lines of code: " (count (split-lines (str
    (slurp 'jo_basic.cpp) 
    (slurp 'jo_basic_async.h)
    (slurp 'jo_basic_b64.h)
    (slurp 'jo_basic_canvas.h)
    (slurp 'jo_basic_gif.h)
    (slurp 'jo_basic_io.h)
    (slurp 'jo_basic_lazy.h) 
    (slurp 'jo_basic_math.h) 
    (slurp 'jo_basic_persistent.h) 
    (slurp 'jo_basic_string.h) 
    (slurp 'jo_basic_system.h)
    (slurp 'jo_gif.cpp) 
    (slurp 'jo_stdcpp.h) 
    (slurp 'pdqsort.h) 
    (slurp 'debugbreak.h) 
    ))))
