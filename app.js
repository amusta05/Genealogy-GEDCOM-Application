'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');
const mysql = require('mysql');
let connection;
let user;
let password;
let tracker = [];
let database;
//const bodyparser = require('body-parser');

//app.use(bodyparser());
app.use(fileUpload());
process.on('uncaughtException', function(err) {

    console.log(err);

});
// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/', function(req, res) {
    res.sendFile(path.join(__dirname + '/public/index.html'));
});

// Send Style, do not change
app.get('/style.css', function(req, res) {
    //Feel free to change the contents of style.css to prettify your Web app
    res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function(req, res) {
    fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function(err, contents) {
        const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {
            compact: true,
            controlFlowFlattening: true
        });
        res.contentType('application/javascript');
        res.send(minimizedContents._obfuscatedCode);
    });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
    if (!req.files) {
        return res.status(400).send('No files were uploaded.');
    }

    let uploadFile = req.files.uploadFile;

    // Use the mv() method to place the file somewhere on your server
    uploadFile.mv('uploads/' + uploadFile.name, function(err) {
        if (err) {
            return res.status(500).send(err);
        }

        res.redirect('/');
    });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req, res) {
    fs.stat('uploads/' + req.params.name, function(err, stat) {
        console.log(err);
        if (err == null) {
            res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
        } else {
            res.send('');
        }
    });
});


let sharedLib = ffi.Library('./sharedLib', {

    'writeToGEDCOM': ['void', ['string', 'string', 'string', 'string']],
    'parseValidGEDCOM': ['string', ['string']],
    'parseIndividualGEDCOM': ['string', ['string']],
    'parseIndividualGEDCOM2': ['string', ['string']],
    'addIndividualWrapper': ['void', ['string', 'string', 'string']],
    'fileNameAndIndWrapper': ['string', ['string']],
    'wrappergListToJSON': ['string', ['string', 'int', 'string', 'string']],
    'wrappergListToJSONAncestors': ['string', ['string', 'int', 'string', 'string']],

});

app.get('/GEDCOM', function(req, res) {

    let a = req.query.f;
    let b = req.query.encoding;
    let c = req.query.subName;
    let d = req.query.subAddress;

    sharedLib.writeToGEDCOM(a, c, d, b);
    res.redirect("/");


});


app.get('/files', function(req, res) {
    let files = fs.readdirSync('./uploads/');
    res.send(files);
});
//dont touch this code.
app.get('/name', function(req, res) {

    let files = fs.readdirSync('./uploads/');

    //console.log(files);

    let array = [];
    for (let i = 0; i < files.length; i++) {

        //console.log('About to parse: ' + files[i]);
        array[i] = sharedLib.parseValidGEDCOM(files[i]);

    }

    let gedFunctions = [];
    for (let i = 0; i < files.length; i++) {
        gedFunctions.push(asynchGEDCOMParse(files[i]));
    }

    Promise.all(gedFunctions)
        .then(function(data) {
            console.log(data);
            return res.send(data);
        })
        .catch(function(err) {
            return res.status(500).send(err);
        });
});
// did not use
function asyncGEDCOMParse(file) {
    return new Promise(function(resolve, reject) {
        let json = sharedLib.parseValidGEDCOM(file);

        if (json.length < 1)
            reject(new Error("Failed to parse file: " + file));
        else
            resolve(json);
    });
}

app.get('/someendpoint', function(req, res) {
    res.send({
        foo: "bar"
    });
});


app.get('/view', function(req, res) {


    let fileName = req.query.filename;
    let array;
    array = sharedLib.parseIndividualGEDCOM(fileName);
    res.send(array);


});

app.get('/indi', function(req, res) {


    let files = req.query.filename;
    let givenName = req.query.givenName;

    let surname = req.query.surname;
    let data = sharedLib.addIndividualWrapper(files, givenName, surname);
    res.send(data);
});
app.get('/desc', function(req, res) {

    let fileName = fs.readdirSync('./uploads/');
    let array = sharedLib.fileNameAndIndWrapper(fileName);
    res.send(array);

});

app.get('/addindi', function(req, res) {


    let option = req.query.countryy;
    let given = req.query.givenName;
    let surn = req.query.surname;

    sharedLib.addIndividualWrapper(option, given, surn);

    //res.send(str);
    res.redirect("/");

});

app.get('/somename', function(req, res) {


    let file = req.query.filename;
    let something = sharedLib.parseValidGEDCOM(file);
    res.send(something);

});
app.get('/options', function(req, res) {


    let file = req.query.filename2;
    let something = sharedLib.fileNameAndIndWrapper(file);
    res.send(something);


});
app.get('/getDesN', function(req, res) {



    let file = req.query.filename;
    let givenName = req.query.gName;
    let surname = req.query.sName;
    let maxGen = req.query.convert;
    let something = sharedLib.wrappergListToJSON(file, maxGen, givenName, surname);
    res.send(something);


});

app.get('/getAncesN', function(req, res) {


    let file = req.query.filename;
    let givenName = req.query.gName;
    let surname = req.query.sName;
    let maxGen = req.query.convert;
    let something = sharedLib.wrappergListToJSONAncestors(file, maxGen, givenName, surname);
    res.send(something);



});
app.get('/options2', function(req, res) {


    let file = req.query.filename2;
    let something = sharedLib.fileNameAndIndWrapper(file);
    res.send(something);


});

app.get('/connection', function(req, res) {

    console.log(req.query.usernamee);
    user = req.query.usernamee;
    let message = "success"
    password = req.query.passwordd;
    database = req.query.databasee;
    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect(function(err) {

        if (err) {

            message = "failure";

        } else {
            message = "success";

        }
        if (database == "") {

            message = "failure";
        }

        connection.query("create table FILE (file_id INT AUTO_INCREMENT PRIMARY KEY,file_name letCHAR(50) NOT NULL, source letCHAR(250) NOT NULL,version letCHAR(10) NOT NULL,encoding letCHAR(10) NOT NULL,sub_name letCHAR(62) NOT NULL,sub_addr letCHAR(256), num_individuals INT,num_families INT)", function(err, rows, fields) {
            if (err) {
                message = "Something went wrong";
            }
        });

        connection.query("create table INDIVIDUAL (file_id INT AUTO_INCREMENT PRIMARY KEY,surname letCHAR(256) NOT NULL,given_name letCHAR(256) NOT NULL,sex letCHAR(1),fam_size INT,source_file INT )", function(err, rows, fields) {
            if (err) {
                
                message = "Something went wrong";
            }
        });


        connection.end();
        res.send({
            message,
        });
    });



});

app.get('/filess', function(req, res) {


    let fileName = fs.readdirSync('./uploads/');
    let array = [];
    let arrayOfIndi = [];
    let temp;
    temp = temp + "[";
    for (let i = 0; i < fileName.length; i++) {

        array[i] = sharedLib.parseValidGEDCOM(fileName[i]);
        if (arrayOfIndi != "") {
            temp = temp + arrayOfIndi + ",";
        }

    }

    res.send({
        array,
        temp,
    });

});
app.get('/getFiles', function(req, res) {

    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();
    let array = req.query.array;
    let indi = req.query.arrayOfIndi;
    let something;
    let record = [];
    let record2 = [];
    let obj;
    let obj2;

    connection.query("delete from FILE", function(err, rows, fields) {


        if (err) {

            console.log("Something went wrong " + err);
        }


    });

    connection.query("delete from INDIVIDUAL", function(err, rows, fields) {


        if (err) {

            console.log("Something went wrong " + err);
        }


    });



    let files = fs.readdirSync('./uploads/');


    for (let i = 0; i < array.length; i++) {

        obj = JSON.parse(array[i]);
        //temp = array[i];
        //console.log(obj);
        let some = "insert into FILE (file_id,file_name,source,version,encoding,sub_name,sub_addr,num_individuals,num_families) values ('";
        let n = i + 1;
        let n2 = n.toString();
        some = some + n2;
        some = some + "','" + obj.fileName;
        some = some + "','" + obj.source;
        some = some + "','" + obj.gedcVersion;
        some = some + "','" + obj.encoding;
        some = some + "','" + obj.subName;
        some = some + "','" + obj.subAddress;
        some = some + "','" + obj.numOfIndi;
        some = some + "','" + obj.numOfFam;

        some = some + "')";

        record.push(some);


    }
    for (let rec of record) {
        connection.query(rec, function(err, rows, fields) {
            if (err) console.log("Something went wrong. " + err);
        });

    }

    for (let i = 0; i < files.length; i++) {
        something = sharedLib.parseIndividualGEDCOM(files[i]);
        obj2 = JSON.parse(something);
        let fileName = files[i];
        for (let j = 0; j < obj2.length; j++) {
            let surname = obj2[j].surname;
            let givenName = obj2[j].givenName;
            let sex = obj2[j].sex;
            let fam_size = obj2[j].familySize;
            let n = j + 1;
            let n2 = n.toString();
            connection.query(`insert into INDIVIDUAL values(null,"${surname}","${givenName}","${sex}",${fam_size},(select FILE.file_id FROM FILE where FILE.file_name = "${fileName}"))`, function(err, rows, fields) {

                if (err) {
					console.log("Something went wrong " +err);
				
                }

            });
        }
    }


});

app.get('/getResult', function(req, res) {


    let x = req.query.temp;
    while (x.charAt(0) == ' ') {
        x = x.substr(1);

    }
    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });
    connection.connect();

    //console.log(x);
    let arr = [];
    let arr2 = [];
    let arr3 = [];
    let arr4 = [];
    let arr5 = [];
    connection.query(x, function(err, rows, fields) {

        if (err) {

            console.log("Something went wrong " + err);
        } else {

            for (let i = 0; i < rows.length; i++) {

                arr[i] = rows[i].given_name;
                arr2[i] = rows[i].surname;
                arr3[i] = rows[i].sex;
                arr4[i] = rows[i].fam_size;
                arr5[i] = rows[i].source_file;	

            }
            res.send({


                arr,
                arr2,
                arr3,
                arr4,
                arr5,

            });

        }


    });


    connection.end()
});

app.get('/clearData', function(req, res) {


    let array = req.query.array;
    let indi = req.query.temp;
    indi = indi.replace("undefined", "");
    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();

    connection.query("delete from FILE", function(err, rows, fields) {
        if (err)
            console.log("Something went wrong. " + err);
        else
            console.log("Cleared data from table");
    });



    connection.query("delete from INDIVIDUAL", function(err, rows, fields) {
        if (err)
            console.log("Something went wrong. " + err);
        else
            console.log("Cleared data from table");
        res.send("Cleared data from table");
    });




    connection.end();

});


app.get('/getStatus', function(req, res) {

    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();

    let messageFile;
    let string;
    let rowSize;
    let rowCount;
    let string2;
    let rowSize2;
    let rowCount2;
    connection.query("select count(file_name) from FILE", function(err, rows, fields) {


        if (err) {

            console.log("Something went wrong")
        } else {
            let obj = JSON.stringify(rows).replace("(file_name)", "");
            let objParsed = JSON.parse(obj);
            messageFile = messageFile = "Database has " + objParsed[0].count + " files";
            res.send({
                messageFile,
            });
        }



    });



    connection.end();
});

app.get('/getStatus2', function(req, res) {

    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();

    let messageFile2;
    let string;
    let rowSize;
    let rowCount;

    connection.query("select sum(num_individuals) from FILE", function(err, rows, fields) {


        if (err) {

            console.log("Something went wrong")
        } else {
            let obj = JSON.stringify(rows).replace("(num_individuals)", "");
            let objParsed = JSON.parse(obj);
            messageFile2 = messageFile2 = " and " + objParsed[0].sum + " individuals.";
            res.send({
                messageFile2,
            });
        }



    });



    connection.end();



});

app.get('/getGivnAndSub', function(req, res) {



    let x = req.query.temp;
    while (x.charAt(0) == ' ') {
        x = x.substr(1);
    }

    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();


    let givenNameArray = [];
    let subArray = [];
    connection.query(x, function(err, rows, fields) {


        if (err) {

            console.log("Something went wrong " + err);
        } else {

            

            for (let i = 0; i < rows.length; i++) {

                givenNameArray[i] = rows[i].given_name;
                subArray[i] = rows[i].sub_name;
            }
            res.send({

                givenNameArray,
                subArray,

            });

        }


    });


    connection.end();

});

app.get('/getSex', function(req, res) {

    let x = req.query.temp;
    while (x.charAt(0) == ' ') {
        x = x.substr(1);
    }

    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();

    //console.log(x);
    let arr = [];
    let arr2 = [];
    let arr3 = [];
    let arr4 = [];
    let arr5 = [];
    connection.query(x, function(err, rows, fields) {

        if (err) {

            console.log("Something went wrong " + err);
        } else {

            

            for (let i = 0; i < rows.length; i++) {

                arr[i] = rows[i].given_name;
                arr2[i] = rows[i].surname;
                arr3[i] = rows[i].sex;
                arr4[i] = rows[i].fam_size;
                arr5[i] = rows[i].source_file;
                	

            }
            res.send({


                arr,
                arr2,
                arr3,
                arr4,
                arr5,

            });

        }


    });




});

app.get('/getAllFemales', function(req, res) {

    let x = req.query.temp;
    
    while (x.charAt(0) == ' ') {
        x = x.substr(1);
    }
    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();
    connection.query(x, function(err, rows, fields) {



        if (err) {

            console.log("Something went wrong " + err);

        } else {

            console.log(rows);
            let obj = JSON.stringify(rows).replace("COUNT(*)", "count");
            let objParsed = JSON.parse(obj);
            //console.log(objParsed[0].count);
            let count = objParsed[0].count;

            res.send({

                count,
            });

        }




    });




});
app.get('/getSingleFile', function(req, res) {


    let x = req.query.temp;

    while (x.charAt(0) == ' ') {
        x = x.substr(1);
    }
    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();
    let arr = [];
    let arr2 = [];
    let arr3 = [];
    let arr4 = [];
    let arr5 = [];

    connection.query(x, function(err, rows, fields) {

        if (err) {

            console.log("Something went wrong " + err);
        } else {

            for (let i = 0; i < rows.length; i++) {

                arr[i] = rows[i].given_name;
                arr2[i] = rows[i].surname;
                arr3[i] = rows[i].sex;
                arr4[i] = rows[i].fam_size;
                arr5[i] = rows[i].source_file;
                //console.log("given name is " + rows[i].given_name);	

            }
            res.send({

                arr,
                arr2,
                arr3,
                arr4,
                arr5,

            });
        }



    });


    connection.end();


});
app.get('/customQuery', function(req, res) {


    let x = req.query.temp;

    while (x.charAt(0) == ' ') {
        x = x.substr(1);
    }
    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();

    let message = "";
    connection.query(x, function(err, rows, fields) {

        

        if (err) {
            message = "Not valid SQL syntax";
            console.log("Something went wrong " + err);
        }

        res.send({
            message,
            rows
        });


    });

    connection.end();

});
app.get('/help', function(req, res) {

    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();


    connection.query("describe INDIVIDUAL", function(err, rows, fields) {

        if (err) {

            console.log("Something went wrong " + err);

        } else {

            res.send({
                rows,
            });
        }


    });



    connection.end();


});
app.get('/helpFILE', function(req, res) {

    connection = mysql.createConnection({
        host: 'dursley.socs.uoguelph.ca',
        user: user,
        password: password,
        database: database
    });

    connection.connect();

    connection.query('describe FILE', function(err, rows, fields) {



        if (err) {

            console.log("Something went wrong " + err);
        } else {

            res.send({
                rows,
            });
        }


    });

    connection.end();



});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);