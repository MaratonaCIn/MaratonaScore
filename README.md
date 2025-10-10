// TO BUILD:

cmake -B build/debug
cmake --build build/debug


// TO RUN:

build\debug\bin\maratona_score.exe convert-batch D:\MaratonaScore\batch-conversions.txt

build\debug\bin\maratona_score.exe batch D:\MaratonaScore\batch-processing.txt

build\debug\bin\maratona_score.exe convert-batch D:\MaratonaScore\batch-conversions.txt && build\debug\bin\maratona_score.exe batch D:\MaratonaScore\batch-processing.txt