<?php

    // configuration
    require("../includes/config.php"); 

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submission
        if (empty($_POST["deposit"]))
        {
            apologize("You must provide a deposit amount.");
        }
        if (htmlspecialchars($_POST["deposit"]) < 0)
        {
            apologize("You can only deposit a positive amount ");
            exit;
        }

        // query database for user
        $newcash = query("UPDATE users SET cash = cash + ? WHERE id = ?", htmlspecialchars($_POST["deposit"]), $_SESSION["id"]);
        header("Location: /");
    }
    else
    {
        // else render form
        render("deposit_form.php", ["title" => "Log In"]);
    }

?>
