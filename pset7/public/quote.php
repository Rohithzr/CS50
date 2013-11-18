<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submission
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a symbol.");
            exit;
        }

        // lookup stocks for symbol
        $s = lookup($_POST["symbol"]);
        if ($s === false)
        {
            apologize("That symbol is invalid");
            exit;
        }
        render("quote.php",["title" => "Quote","price" => $s["price"], "name" => $s["name"], "symbol" => $s["symbol"]] );


       
    }
    else
    {
        // else render form
        render("quote_form.php", ["title" => "Quote"]);
    }

?>
