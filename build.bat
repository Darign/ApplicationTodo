:: Creación de los directorios
MD .\web\backend\
MD .\web\backend\database\
MD .\web\backend\lib\
MD .\web\backend\lib\actions\

:: Creación de los dll
call gcc -shared -I./vendors/ ./vendors/sqlite/sqlite-amalgamation/sqlite3.c -o      ./sqlite3.dll

call g++ -shared -Wall -std=c++11 -fPIC -I./include/ -I./vendors -I./src/ ./src/Validators/Validator.cpp -o       ./web/backend/lib/Validator.dll
call g++ -shared -Wall -std=c++11 -fPIC -I./include/ -I./vendors/ ./src/RequestHandler/RequestHandler.cpp -o      ./web/backend/lib/RequestHandler.dll    
call g++ -shared -Wall -std=c++11 -fPIC -I./include/ -I./vendors/ ./src/ResponseHandler/ResponseHandler.cpp -o    ./web/backend/lib/ResponseHandler.dll
call g++ -shared -Wall -std=c++11 -fPIC -I./include/ -I./vendors/ -I./src/ ./src/Actions/CreateNote.cpp -o        ./web/backend/lib/actions/CreateNote.dll ./sqlite3.dll 
call g++ -shared -Wall -std=c++11 -fPIC -I./include/ -I./vendors/ -I./src/ ./src/Actions/DeleteNote.cpp -o        ./web/backend/lib/actions/DeleteNote.dll ./sqlite3.dll
call g++ -shared -Wall -std=c++11 -fPIC -I./include/ -I./vendors/ -I./src/ ./src/Actions/UpdateNote.cpp -o        ./web/backend/lib/actions/UpdateNote.dll ./sqlite3.dll
call g++ -shared -Wall -std=c++11 -fPIC -I./include/ -I./vendors/ -I./src/ ./src/Actions/ReadNotes.cpp -o        ./web/backend/lib/actions/ReadNotes.dll ./sqlite3.dll
call g++ -shared -Wall -std=c++11 -fPIC -I./include/ -I./vendors -I./src ./src/Application/Application.cpp -o       ./web/backend/lib/Application.dll ./sqlite3.dll

call g++ -Wall -fPIC -std=c++11 -I./include/ -I./vendors/ -I./src main.cpp -o ./index.cgi