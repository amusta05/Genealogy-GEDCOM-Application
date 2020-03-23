// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {


    let someOption;
    let someOption2;
    let someOption3;

    // On page-load AJAX Example
    $.ajax({
        type: 'get', //Request type
        dataType: 'json', //Data type - we will use JSON for almost everything 
        url: '/someendpoint', //The server endpoint we are connecting to
        success: function(data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */

            //We write the object to the console to show that the request was successful
            console.log(data);
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });


    $('#help').click(function(e) {

        e.preventDefault();
        let flag = 0;
        //console.log("helpbUTTON");

        if (flag == 0) {
            $.ajax({
                type: 'get', //Request type
                // dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/helpFILE', //The server endpoint we are connecting to
                success: function(data) {

                    //console.log(data.rows);
                    flag = 1;
                    $('#results').empty();
                    $('#results').append('<tr align = \"center\"><th style=\"width:200%\" align = \"center\">' + 'FILES' + '</th></tr>');
                    $('#results').append('<tr align= \"center\"><td align = \"center>' + 'Field' + '</td><td align = \"center\">' + 'Type' + '</td><td align = \"center\">' + 'NULL' + '</td><td align = \"center\">' + 'Key' + '</td><td align = \"center\">' + 'Deafault' + '</td><td align = \"center\">' + 'Extra' + '</tr>');

                    for (let i = 0; i < data.rows.length; i++) {
                        $('#results').append('<tr align= \"center\"><td align = \"center>' + data.rows[i].Field + '</td><td align = \"center\">' + data.rows[i].Type + '</td><td align = \"center\">' + data.rows[i].Null + '</td><td align = \"center\">' + data.rows[i].Key + '</td><td align = \"center\">' + data.rows[i].Default + '</td><td align = \"center\">' + data.rows[i].Extra + '</tr>');

                    }
                    $.ajax({
                        type: 'get', //Request type
                        //dataType: 'json',       //Data type - we will use JSON for almost everything 
                        url: '/help', //The server endpoint we are connecting to
                        success: function(data) {
                            //$('#results').empty();
                            //console.log("came here");
                            $('#results').append('<tr align = \"center\"><th style=\"width:200%\" align = \"center\">' + 'INDIVIDUALS' + '</th></tr>');
                            $('#results').append('<tr align= \"center\"><td align = \"center>' + 'Field' + '</td><td align = \"center\">' + 'Type' + '</td><td align = \"center\">' + 'NULL' + '</td><td align = \"center\">' + 'Key' + '</td><td align = \"center\">' + 'Deafault' + '</td><td align = \"center\">' + 'Extra' + '</tr>');

                            for (let i = 0; i < data.rows.length; i++) {
                                $('#results').append('<tr align= \"center\"><td align = \"center>' + data.rows[i].Field + '</td><td align = \"center\">' + data.rows[i].Type + '</td><td align = \"center\">' + data.rows[i].Null + '</td><td align = \"center\">' + data.rows[i].Key + '</td><td align = \"center\">' + data.rows[i].Default + '</td><td align = \"center\">' + data.rows[i].Extra + '</tr>');

                            }
                            flag = 0;

                        },
                        fail: function(error) {
                            // Non-200 return, do something with error
                            console.log(error);
                        }

                    });


                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }

            });
        }




    });
    $('#select_button').click(function(e) {


        e.preventDefault();
        let x = document.getElementById("select_choice");
        someOption = x.options[x.selectedIndex].text;
        //console.log(x.value);

        let y = document.getElementById("drop");
        someOption2 = y.options[y.selectedIndex].text;

        //console.log(someOption2);

        if (someOption2 == "Display individuals from a specfic file") {
            $('#query_textbox').empty();
            $('#query_textbox').append('SELECT * from INDIVIDUAL where source_file IN (SELECT file_id from FILE where file_Name = \"' + someOption + '\")');
        } else if (someOption2 == "Display all Males in a specfic file") {
            e.preventDefault();
            let x = document.getElementById("select_choice2");
            someOption3 = x.options[x.selectedIndex].text;
            //console.log("some option is " +someOption);
            $('#query_textbox').empty();
            $('#query_textbox').append('select * from INDIVIDUAL where INDIVIDUAL.sex  = "M" AND source_file IN (SELECT file_id from FILE where file_Name = \"' + someOption3 + '\")');

        }


    });

    $('#submit_button').click(function(e) {


        let x = $("#query_textbox").val();
        //console.log("x is " +x);



        // On page-load AJAX Example


        if (x == "SELECT * FROM INDIVIDUAL ORDER BY surname") {

            $.ajax({
                type: 'get', //Request type
                //dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/getResult',
                data: {
                    temp: x
                },
                success: function(data) {

                    $('#results').empty();
                    $('#results').append('<tr align = \"center\"> <td align = "center">' + 'Given Name' + '</td> <td align = \"center\" >' + 'Surname' + '</td> <td align = "center">' + 'Sex' + '</td> <td align = "center">' + 'Fam Size' + '</td> <td align = "center">' + 'Souce file' + '</td></tr>');
                    for (let i = 0; i < data.arr.length; i++) {



                        $('#results').append('<tr align = \"center\" > <td = align = "center" >' + data.arr[i] + '</td> <td align = \"center\">' + data.arr2[i] + '</td> <td align = \"center\">' + data.arr3[i] + '</td> <td align = \"center\">' + data.arr4[i] + '</td> <td align = \"center\">' + data.arr5[i] + '</td> </tr>');

                    }

                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }
            });



        } else if (x == "select INDIVIDUAL.given_name ,FILE.sub_name FROM INDIVIDUAL JOIN FILE ON INDIVIDUAL.source_file = FILE.file_id") {
            // On page-load AJAX Example
            $.ajax({
                type: 'get', //Request type
                //dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/getGivnAndSub',
                data: {
                    temp: x
                },
                success: function(data) {


                    $('#results').empty();
                    $('#results').append('<tr align = \"center\">' + '<td align = \"center\">' + 'Given Name' + '</td> <td align = \"center\">' + 'Submitter Name' + '</td></tr>');
                    for (let i = 0; i < data.givenNameArray.length; i++) {

                        $('#results').append('<tr align = \"center\">' + '<td align = \"center\">' + data.givenNameArray[i] + '</td> <td align = \"center\">' + data.subArray[i] + '</td></tr>');

                        //console.log("it is " +data.givenNameArray[i]);


                    }
                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }
            });


        } else if (x == 'select * from INDIVIDUAL where INDIVIDUAL.sex  = "M" AND source_file IN (SELECT file_id from FILE where file_Name = \"' + someOption3 + '\")') {

            $.ajax({
                type: 'get', //Request type
                //dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/getSex',
                data: {
                    temp: x
                },
                success: function(data) {

                    $('#results').empty();
                    $('#results').append('<tr align = \"center\"> <td align = "center">' + 'Given Name' + '</td> <td align = \"center\" >' + 'Surname' + '</td> <td align = "center">' + 'Sex' + '</td> <td align = "center">' + 'Fam Size' + '</td> <td align = "center">' + 'Souce file' + '</td></tr>');
                    for (let i = 0; i < data.arr.length; i++) {



                        $('#results').append('<tr align = \"center\" > <td = align = "center" >' + data.arr[i] + '</td> <td align = \"center\">' + data.arr2[i] + '</td> <td align = \"center\">' + data.arr3[i] + '</td> <td align = \"center\">' + data.arr4[i] + '</td> <td align = \"center\">' + data.arr5[i] + '</td> </tr>');

                    }

                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }
            });


        } else if (x == "SELECT COUNT(*) from INDIVIDUAL where INDIVIDUAL.sex = \"F\"") {

            $.ajax({
                type: 'get', //Request type
                // dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/getAllFemales',
                data: {
                    temp: x
                },
                success: function(data) {


                    $('#results').empty();
                    $('#results').append('<tr align = \"center\"> <td align = \"center\" >' + 'Count' + '</td> </tr>');
                    $('#results').append('<tr align = \"center\"> <td align = \"center\" >' + data.count + '</td> </tr>');

                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }
            })


        } else if (x == 'SELECT * from INDIVIDUAL where source_file IN (SELECT file_id from FILE where file_Name = \"' + someOption + '\")') {
            $.ajax({
                type: 'get', //Request type
                //dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/getSingleFile',
                data: {
                    temp: x
                },
                success: function(data) {


                    $('#results').empty();
                    $('#results').append('<tr align = \"center\"> <td align = "center">' + 'Given Name' + '</td> <td align = \"center\" >' + 'Surname' + '</td> <td align = "center">' + 'Sex' + '</td> <td align = "center">' + 'Fam Size' + '</td> <td align = "center">' + 'Souce file' + '</td></tr>');
                    for (let i = 0; i < data.arr.length; i++) {



                        $('#results').append('<tr align = \"center\" > <td = align = "center" >' + data.arr[i] + '</td> <td align = \"center\">' + data.arr2[i] + '</td> <td align = \"center\">' + data.arr3[i] + '</td> <td align = \"center\">' + data.arr4[i] + '</td> <td align = \"center\">' + data.arr5[i] + '</td> </tr>');

                    }
                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }
            });


        } else {

            $.ajax({
                type: 'get', //Request type
                //dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/customQuery',
                data: {
                    temp: x
                },
                success: function(data) {

                    if (data.message == "Not valid SQL syntax") {

                        alert("Not valid SQL syntax");

                    } else {

                        //console.log(data.rows);
                        let arr1 = Object.keys(data.rows[0]);
                        //console.log(arr1[0][0]);
                        $('#results').empty();
                        $('#results').append('<tr align = \"center\">');
                        for (let i = 0; i < arr1.length; i++) {
                            $('#results').append('<td align = \"center\">' + arr1[i] + '</td>');

                        }
                        $('#results').append('</tr>');


                        let arr2;

                        //$('#results').empty();


                        for (let i = 0; i < data.rows.length; i++) {

                            $('#results').append('<tr align = \"center\">');
                            arr2 = Object.values(data.rows[i]);

                            for (let j = 0; j < arr2.length; j++) {

                                $('#results').append('<td align = \"center\">' + arr2[j] + '</td>');

                            }


                            $('#results').append('</tr>');

                        }



                    }



                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }
            });

        }

    });

    $('#drop').click(function(e) {
        let option = document.getElementById("drop");
        //console.log('option is' + option.value);

        if (option.value == "Display all individuals sorted by last name") {
            //document.getElementById("query_textbox").readOnly = true;
            $('#query_textbox').empty();
            $('#query_textbox').append('SELECT * FROM INDIVIDUAL ORDER BY surname');
            document.getElementById("select_button").disabled = true;
        }

        if (option.value == "Display Given Name and Submitter Name of all files") {
            $('#enter_info').empty();
            //document.getElementById("query_textbox").readOnly = true;
            $('#query_textbox').empty();
            $('#query_textbox').append('select INDIVIDUAL.given_name ,FILE.sub_name FROM INDIVIDUAL JOIN FILE ON INDIVIDUAL.source_file = FILE.file_id');
            document.getElementById("select_button").disabled = true;
        }
        if (option.value == "Display individuals from a specfic file") {
            $('#enter_info').empty();
            //document.getElementById("query_textbox").readOnly = true;
            document.getElementById("select_button").disabled = false;


        }
        if (option.value == "Display all Males in a specfic file") {
            $('#enter_info').empty();
            document.getElementById("select_button").disabled = false;

        }
        if (option.value == "Display total number of Females") {

            //console.log("HAHAHAHAH")
            $('#enter_info').empty();

            $('#query_textbox').empty();
            $('#query_textbox').append('SELECT COUNT(*) from INDIVIDUAL where INDIVIDUAL.sex = \"F\"');
            document.getElementById("select_button").disabled = true;
            //sdocument.getElementById("query_textbox").readOnly = true;
        }

        if (option.value == "Custom Command") {
            document.getElementById("query_textbox").readOnly = false;
            $('#query_textbox').empty();
            $('#enter_info').empty();
            $('#enter_info').append("<h6 align = \"center\">" + 'Enter Custom Command on textbox' + '</h6>');
            $('#query_textbox').append('SELECT ');
            document.getElementById("select_button").disabled = true;

        }
    });



    $('#clear_data').click(function(e) {


        $.ajax({
            type: 'get', //Request type
            dataType: 'json', //Data type - we will use JSON for almost everything 
            url: '/filess', //The server endpoint we are connecting to
            success: function(data) {

                $('#textarea').empty();
                $('#textarea').append('Data has been cleared.Table was not dropped however');
                $.ajax({
                    type: 'get', //Request type
                    dataType: 'json', //Data type - we will use JSON for almost everything 
                    url: '/clearData',
                    data: {
                        array: data.array,
                        temp: data.temp
                    },
                    success: function(data) {


                        $('#textarea').append(data);
                        alert(data);


                        console.log(data);
                    },
                    fail: function(error) {
                        // Non-200 return, do something with error
                        console.log(error);
                    }
                });




            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });



    });

    $('#display_status').click(function(e) {


        $.ajax({
            type: 'get', //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/getStatus', //The server endpoint we are connecting to
            success: function(data) {

                //alert(data.messageFile);
                //console.log("messageFile is " +data.messageFile);
                $('#textarea').empty();
                $('#textarea').append(data.messageFile);

                $.ajax({
                    type: 'get', //Request type
                    //dataType: 'json',       //Data type - we will use JSON for almost everything 
                    url: '/getStatus2', //The server endpoint we are connecting to
                    success: function(data) {

                        //alert(data.messageFile);
                        // console.log("messageFile is " +data.messageFile2);
                        $('#textarea').append(data.messageFile2);
                    },
                    fail: function(error) {
                        // Non-200 return, do something with error
                        console.log(error);
                    }
                });
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });




    });

    $('#someButton').click(function(e) {

        document.getElementById("clear_data").disabled = false;
        document.getElementById("display_status").disabled = false;
        document.getElementById("execute_query").disabled = false;

        $.ajax({
            type: 'get', //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/filess', //The server endpoint we are connecting to
            success: function(data) {

                $('#textarea').empty();
                $('#textarea').append("Stored all the files");
                //alert("Stored all files");

                //console.log("data is " + data);
                $.ajax({
                    type: 'get', //Request type
                    //dataType: 'json',       //Data type - we will use JSON for almost everything 
                    url: '/getFiles',
                    data: {
                        array: data.array,
                        arrayOfIndi: data.temp
                    },
                    success: function(data) {



                        //console.log(data.temp); 
                    },
                    fail: function(error) {
                        // Non-200 return, do something with error
                        console.log(error);
                    }
                });

            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });
    });
    $('#login_database').click(function(e) {

        e.preventDefault();
        //console.log("it comes here");
        document.getElementById("myBtn").disabled = true;
        document.getElementById("someButton").disabled = false;

        let user = $('#username').val();

        let pass = $('#password').val();

        let datab = $('#database').val();

        //console.log(user);
        //console.log(pass);
        //console.log(datab);
        $.ajax({
            type: 'get', //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/connection',
            data: {
                usernamee: user,
                passwordd: pass,
                databasee: datab
            },
            success: function(data) {


                //console.log(data.message);

                if (data.message == "success") {

                    //alert("hahahah"); 	
                    myModal.style.display = "none";
                    $('#textarea').empty();
                    $('#textarea').append('Login Successful');

                } else {

                    alert("Login Failed");
                }


            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });




    });

    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e) {
        e.preventDefault();
        $.ajax({});
    });
    $('#viewPanel').submit(function(e) {

        e.preventDefault();
        let x = document.getElementById("viewselect");
        let option = x.options[x.selectedIndex].text;



        if (option != "select a file") {

            //console.log("went into the if statement");
            $.ajax({
                type: 'get',
                //dataType: 'json',
                url: '/view',
                data: {
                    filename: option
                },
                success: function(data) {
                    //console.log("the data is" +data);
                    let obj = JSON.parse(data);
                    //console.log("it works!!");
                    // console.log(data);

                    $('#customerss').empty();
                    $('#customerss').append('<tr align = "\center\" > <td = align = \"center\">' + 'Given Name' + '</td><td = align = \"center\">' + 'Surname' + '</td><td = align = \"center\">' + 'Sex' + '</td><td = align = \"center\">' + 'Family Size' + '</td></tr>')
                    // $('#customerss').append('<tr align = \"center\"> </tr>');
                    for (let i = 0; i < obj.length; i++) {

                        ($('#customerss')).append('<tr align = \'center\'> <th align = \'center\'>' + obj[i].givenName + '</th> <th align  = "center">' + obj[i].surname + '</th> <th align  = "center">' + obj[i].sex + '</th>  <th align  = "center">' + obj[i].familySize + '</th> </tr>');


                    }
                    $('#textarea').empty();
                    $('#textarea').append(option + " was selected");

                },
                fail: function(error) {

                    console.log(error);

                }




            });

        }

    });



    $('#choosefile').click(function(e) {

        e.preventDefault();
        let x = document.getElementById("desc");
        let option = x.options[x.selectedIndex].text;



        $.ajax({
            type: 'get',
            url: 'options',
            data: {
                filename2: option,
            },
            success: function(data) {

                let obj = JSON.parse(data);
                //console.log(obj.length);
                $('#de').empty();
                for (let i = 0; i < obj.length; i++) {


                    $('#de').append('<option>' + obj[i].givenName + ' ' + obj[i].surname + '</options>');


                }

                //console.log(data); 
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }



        });


    });
    $('#choosefile2').click(function(e) {

        e.preventDefault();
        let x = document.getElementById("desc2");
        let option = x.options[x.selectedIndex].text;



        $.ajax({
            type: 'get',
            url: 'options2',
            data: {
                filename2: option,
            },
            success: function(data) {

                let obj = JSON.parse(data);
                // console.log(obj.length);
                $('#de2').empty();
                for (let i = 0; i < obj.length; i++) {


                    $('#de2').append('<option>' + obj[i].givenName + ' ' + obj[i].surname + '</options>');


                }

                //console.log(data); 
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }



        });


    });

    $('#getD').click(function(e) {

        e.preventDefault();
        let y = document.getElementById("desc");
        let option2 = y.options[y.selectedIndex].text;
        //console.log(option2);

        let x = document.getElementById('de');
        let option = x.options[x.selectedIndex].text;
        //console.log(option);
        let str = option.split(" ");
        //console.log("option is " +option);
        //console.log("str[0] is " +str[0]);
        let givenName = str[0];


        //console.log("given name is " +givenName);
        let surname = str[1];
        if (surname == null && givenName == "Unknown") {

            surname = "Unknown";
            givenName = "";
        }
        //console.log("surname is " +surname);

        let z = document.getElementById("numGen").value;

        let option3 = $('#numGen').val();
        let convert = parseInt(option3);


        //console.log("convert is " +convert);

        $.ajax({
            type: 'get', //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/getDesN',
            data: {
                filename: option2,
                gName: givenName,
                sName: surname,
                gen: convert
            }, //The server endpoint we are connecting to
            success: function(data) {


                //We write the object to the console to show that the request was successful
                //console.log("the data is" +data); 

                //console.log(obj);
                //console.log("data is " +data);
                let obj = JSON.parse(data);
                $('#someid').find('tr:not(:first)').remove();
                // $('#someid').empty(); //COME BACK TO THIS.
                //$('#someid').append('<tr><td>' +Generation+'</td></tr>');
                let i = 1;
                for (j in obj) {

                    let gen = "";
                    let list = obj[j];
                    for (index in list) {
                        gen += list[index].givenName + " " + list[index].surname + ", ";

                    }
                    $('#someid tr:last').after('<tr>' + '<td>' + i + '<td>' + '<td>' + gen + '</td>' + '</tr>');
                    i++;

                }
                $('#textarea').empty();
                $('#textarea').append('Descendants added');



            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });

    });



    $('#getA').click(function(e) {

        e.preventDefault();
        let y = document.getElementById("desc2");
        let option2 = y.options[y.selectedIndex].text;
        //console.log(option2);

        let x = document.getElementById('de2');
        let option = x.options[x.selectedIndex].text;

        let str = option.split(" ");
        let givenName = str[0];
        let surname = str[1];
        if (surname == null && givenName == "Unknown") {

            surname = "Unknown";
            givenName = "";
        }
        let z = document.getElementById("numGen2").value;

        let option3 = $('#numGen2').val();
        let convert = parseInt(option3);
        //console.log("convert is " +convert);

        $.ajax({
            type: 'get', //Request type
            //dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/getAncesN',
            data: {
                filename: option2,
                gName: givenName,
                sName: surname,
                gen: convert
            }, //The server endpoint we are connecting to
            success: function(data) {


                //We write the object to the console to show that the request was successful
                //console.log("the data is" +data); 

                //console.log(obj);

                let obj = JSON.parse(data);
                // $('#someid').empty(); //COME BACK TO THIS.
                //$('#someid').append('<tr><td>' +Generation+'</td></tr>');
                let i = 1;
                for (j in obj) {

                    let gen = "";
                    let list = obj[j];
                    for (index in list) {
                        gen += list[index].givenName + " " + list[index].surname + ", ";

                    }
                    $('#someid2 tr:last').after('<tr>' + '<td>' + i + '<td>' + '<td>' + gen + '</td>' + '</tr>');
                    i++;

                }
                $('#textarea').empty();
                $('#textarea').append('Ancestors added');


            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });

    });




    $.ajax({
        type: 'get', //Request type
        //dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/files', //The server endpoint we are connecting to
        success: function(data) {
            let emp = "";
            //console.log(data.length);
            for (let i = 0; i < data.length; i++) {

                emp += "<option>" + data[i] + "</option>"

            }
            //console.log(data);
            let drop = document.getElementById('viewselect');
            let drop1 = document.getElementById('countryy');
            let drop2 = document.getElementById('desc');
            let drop3 = document.getElementById('desc2');
            let drop4 = document.getElementById('select_choice');
            let drop5 = document.getElementById('select_choice2');
            drop.innerHTML = emp;
            drop1.innerHTML = emp;
            drop2.innerHTML = emp;
            drop3.innerHTML = emp;
            drop4.innerHTML = emp;
            drop5.innerHTML = emp;

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);

        }
    });



    function function1() {

        $.ajax({
            type: 'get', //Request type
            dataType: 'json', //Data type - we will use JSON for almost everything 
            url: '/files', //The server endpoint we are connecting to
            success: function(data) {


                //console.log("something");
                function2(data);
            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });




    }



    function function2(data) {



        //console.log("length is " +data.length);

        for (let i = 0; i < data.length; i++) {

            //console.log(data[i]);
            $.ajax({
                type: 'get', //Request type
                dataType: 'json', //Data type - we will use JSON for almost everything 
                url: '/somename', //The server endpoint we are connecting to
                data: {
                    filename: data[i],

                },
                success: function(data) {

                    
                    $('#customers').append('<tr align = \"center\" > <td align = \"center\"> <a  href=\"./uploads/' + data.fileName + '\">' + data.fileName + '</a></td> <td align = \"center\" >' + data.source + '</td> <td  align = \"center\">' + data.gedcVersion + '</td> <td  align = \"center\">' + data.encoding + '</td><td  align = \"center\">' + data.subName + '</td><td  align = \"center\">' + data.subAddress + '</td><td  align = \"center\">' + data.numOfIndi + '</td><td  align = \"center\">' + data.numOfFam + '</td></tr>')
                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error);
                }
            });


        }




    }

    function1();



    let modal = document.getElementById('myModal');

    // Get the button that opens the modal
    let btn = document.getElementById("myBtn");
    console.log(btn);
    // Get the <span> element that closes the modal
    let span = document.getElementsByClassName("close")[0];

    // When the user clicks the button, open the modal 
    btn.onclick = function() {
        console.log("comes here");
        modal.style.display = "block";
    }

    // When the user clicks on <span> (x), close the modal
    span.onclick = function() {
        modal.style.display = "none";
    }

    // When the user clicks anywhere outside of the modal, close it
    window.onclick = function(event) {
        if (event.target == modal) {
            modal.style.display = "none";
        }
    }



    function toggle() {

        let first = document.getElementById('login_database'),
            second = document.getElementById('second');

        if (first.style.display === "block") {

            first.style.display = "none";
            second.style.display = "block";

        } else { // switch back

            first.style.display = "block";
            second.style.display = "none";

        }

    }

    let modal2 = document.getElementById('myModal2');

    // Get the button that opens the modal
    let btn2 = document.getElementById("execute_query");

    // Get the <span> element that closes the modal
    let span2 = document.getElementsByClassName("closee")[0];

    // When the user clicks the button, open the modal 
    btn2.onclick = function() {
        modal2.style.display = "block";
    }

    // When the user clicks on <span> (x), close the modal
    span2.onclick = function() {
        modal2.style.display = "none";
    }

    // When the user clicks anywhere outside of the modal, close it
    window.onclick = function(event) {
        if (event.target == modal2) {
            modal2.style.display = "none";
        }
    }




});

function validate() {


    let x = document.forms["myform"]["f"];
    //console.log("coming here");
    let some = document.getElementById('para');
    let flag = 0;
    let flag1 = 0;
    let otherFlag = 0;

    //document.write(x);
    $('#textarea').empty();
    if (x.value == "") {

        x.style.border = "2px solid red";
        document.getElementById("para").style.color = "red";
        some.textContent = "File required";
        x.focus();

        //alert("File name must not be empty");
        flag = 1;
        otherFlag = 1;
        //return false;

    } else {
        x.style.border = "0px";
        some.textContent = "";

    }
    let index = x.value.indexOf(".ged");
    if (index == -1) {
        x.style.border = "2px solid red";
        document.getElementById("para").style.color = "red";
        some.textContent = "Not a valid GEDCOM file";
        $('#textarea').append('Not a valid GEDCOM file. ');
        x.focus();

        flag = 1;


    } else {
        x.style.border = "0px";
        some.textContent = "";

    }
    let y = document.forms["myform"]["subName"];
    let subNameError = document.getElementById('subN');
    if (y.value == "") {

        y.style.border = "2px solid red";
        document.getElementById("subN").style.color = "red";
        subNameError.textContent = "Submitter Name required";
        $('#textarea').append('Submitter Name is required ');
        y.focus();
        flag1 = 1;
        //alert("File name must not be empty");
        //return false;

    } else {
        y.style.border = "0px";
        subNameError.textContent = "";

    }
    if (flag == 1 || flag1 == 1) {

        return false;
    }
}

function validateIndi() {


    let gN = document.forms["viewPanell"]["givenName"];
    let gNerror = document.getElementById('gname');
    let sN = document.forms["viewPanell"]["surname"];
    let sNerror = document.getElementById('sname');
    let flag = 0;
    let flag1 = 0;
    $('#textarea').empty();
    //document.write(str);
    if (gN.value == "") {

        gN.style.border = "2px solid red";
        document.getElementById("gname").style.color = "red";

        $('#textarea').append('Given Name is required. ' + '<br>');
        gNerror.textContent = "Given Name is required";
        gN.focus();

        flag = 1;


    } else {

        gN.style.border = "0px";
        gNerror.textContent = "";
    }

    if (sN.value == "") {

        sN.style.border = "2px solid red";
        document.getElementById("sname").style.color = "red";
        sNerror.textContent = "Surname is required";
        $('#textarea').append('Surname is required');
        sN.focus();

        flag1 = 1;


    } else {

        sN.style.border = "0px";
        sNerror.textContent = "";
    }

    if (flag == 1 || flag1 == 1) {

        return false;
    }
}

function validateGen() {


    let maxGen = document.forms["someForm"]["numGen"];
    let xerror = document.getElementById('numGenDiv');
    let flag = 0;
    let flag2 = false;

    if (maxGen.value == "" || isNaN(maxGen.value) || parseInt(maxGen.value) < 0) {

        maxGen.style.border = "2px solid red";
        document.getElementById("numGenDiv").style.color = "red";
        xerror.textContent = "Invalid Max Gen";
        maxGen.focus();

        flag = 1;


    } else {
        maxGen.style.border = "0px";
        xerror.textContent = "";

    }



    if (flag == 1) {

        return false;
    }



}

function validateGen2() {

    let maxGen = document.forms["someForm2"]["numGen1"];
    let xerror = document.getElementById('numGenDiv1');
    let flag = 0;
    let flag2 = false;

    if (maxGen.value == "" || isNaN(maxGen.value) || parseInt(maxGen.value) < 0) {

        maxGen.style.border = "2px solid red";
        document.getElementById("numGenDiv1").style.color = "red";
        xerror.textContent = "Invalid Max Gen";
        maxGen.focus();

        flag = 1;


    } else {
        maxGen.style.border = "0px";
        xerror.textContent = "";

    }



    if (flag == 1) {

        return false;
    }



}

function upload() {

    let file = document.forms['uploadform']['myFile'];
    let error = document.getElementById('myFileError')
    if (file.value != "") {
        let n = file.value.includes(".ged", file.value.length - 5);
        let c = ".ged";
        let flag = 0;
        if (n == false) {

            file.style.border = "2px solid red";
            document.getElementById('myFileError');
            error.textContent = "Not a Valid GEDCOM file";
            $('#textarea').empty();
            $('#textarea').append('Not a valid GEDCOM');
            file.focus();
            flag = 1;
        }


    }

    if (flag == 1) {

        return false;
    }


}

function validateDatabase() {
    let username = document.forms['databaseform']['username'];
    let error = document.getElementById('username_error');

    let flag = 0;

    let password = document.forms['databaseform']['password'];
    let error2 = document.getElementById('password_error');
    let flag2 = 0;

    let database = document.forms['databaseform']['database'];
    let error3 = document.getElementById('database_error');
    let flag3 = 0;
    if (username.value == "") {

        username.style.border = "2px solid red";
        document.getElementById("username_error").style.color = "red";
        error.textContent = "Username is required";
        username.focus();

        flag = 1;
    }

    if (password.value == "") {

        password.style.border = "2px solid red";
        document.getElementById("password_error").style.color = "red";
        error2.textContent = "Password is required";
        password.focus();

        flag2 = 1;
    }
    if (database.value == "") {
        database.style.border = "2px solid red";
        document.getElementById("database_error").style.color = "red";
        error3.textContent = "Databse name is required";
        database.focus();

        flag3 = 1;


    }


    if (flag == 1 || flag2 == 1 || flag3 == 1) {
        return false;

    }



}



function dropDown() {
    document.getElementById("myDropdown").classList.toggle("show");
}



function check() {
    let dropdown = document.getElementById("drop");
    let current_value = dropdown.options[dropdown.selectedIndex].value;

    if (current_value == "Display individuals from a specfic file") {


        document.getElementById("select_choice").style.display = "block";
    } else {
        document.getElementById("select_choice").style.display = "none";
    }

    let dropdown2 = document.getElementById("drop");
    let current_value2 = dropdown.options[dropdown.selectedIndex].value;

    if (current_value2 == "Display all Males in a specfic file") {


        document.getElementById("select_choice2").style.display = "block";
    } else {
        document.getElementById("select_choice2").style.display = "none";
    }



}