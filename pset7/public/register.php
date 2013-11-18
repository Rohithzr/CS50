<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
       if(empty($_POST["password"]))
       {
           apologize("Please enter a password");
       	   exit;
       }
        if(empty($_POST["confirmation"]))
       {
           apologize("Please confirm your password");
       	   exit;
       }
       if($_POST["password"] != $_POST["confirmation"])
       {
		apologize("Please make sure your password and combination match");	
		exit;       
       }
       if(empty($_POST["username"]))
       {
		apologize("Please make sure to enter a user name");
		exit;
       }
       
       $result = query("INSERT INTO users (username, hash, cash) VALUES (?, ?, 10000.0000)", $_POST["username"], crypt($_POST["password"]));
       if ($result === false)
       {
	        apologize("That user name is already taken");
	        exit;
       }
       
       $rows = query("SELECT LAST_INSERT_ID() AS id");
       $id = $rows[0]["id"];
       $_SESSION["id"] = $id;
       header("Location: /index.php");
      
    }
    else
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

?>
