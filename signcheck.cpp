#include <pthread.h>
#include <crow.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "key.h"

//g++ signcheck.cpp -o signcheck -lpthread -DCROW_ENABLE_SSL -lssl -lcrypto -lcryptopp
using namespace std;

int listener(){

    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([](){auto page = crow::mustache::load_text("index.html");return page;});
    CROW_ROUTE(app, "/requestjson.js")([]{
    crow::mustache::context ctx;
    return crow::mustache::load_text("requestjson.js");
    });
    CROW_ROUTE(app, "/signer.js")([](const crow::request &req){

        crow::mustache::context ctx;
        return crow::mustache::load_text("signer.js");
        
    });


    CROW_ROUTE(app, "/verifysign").methods("POST"_method)([](const crow::request &req, crow::response &res) {

        auto x = crow::json::load(req.body);

        if (!x) {
            res.set_header("Content-Type", "text/plain");
            res.body = "Bad Format";
            return res.end();
        }
        // Abrir el archivo que se va a descargar
         string publicDir=  x["pub"].s();

        if(verifySignature(x["msg"].s(), x["sig"].s(), loadPublicKey(publicDir.substr(2,128)))){
            res.set_header("Content-Type", "text/plain");
            res.body = "OK";
            return res.end();
        }

        res.code = 200;
        res.write("wrong signature");
        res.end();

    });

    app.port(12312).run();
    

    return 0;

}

int main(){
listener(); 
 
return 0;

}