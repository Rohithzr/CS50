<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submission
        if (empty($_POST["stock"]))
        {
            apologize("You must provide a stock name.");
            exit;
        }

        // lookup stocks for symbol
        $s = lookup(htmlspecialchars($_POST["stock"]));
        
        // query the database for the number of shares the user has of that stock
        $shares = query("SELECT shares FROM stocks WHERE id = ? AND symbol = ?", $_SESSION["id"], htmlspecialchars($_POST["stock"]));
        $share = $shares[0]["shares"]; 
        
        // calculate the value of the stocks to be sold
        $valuesold =  $s["price"]* $share; 
        
        // update new amount of cash
        $newcash = query("UPDATE users SET cash = cash + ? WHERE id = ?", $valuesold, $_SESSION["id"]);
        
        // delete stocks from database
        $deleter = query("DELETE FROM stocks WHERE id = ? AND symbol = ?", $_SESSION["id"], htmlspecialchars($_POST["stock"]));
        
        // add the transaction to the history database
        $updatehistory = query("INSERT INTO history (type, symbol, shares, price, id) VALUES(?, ?, ?, ?, ?)", 'SELL', htmlspecialchars($_POST["stock"]), $share, $s["price"], $_SESSION["id"]);
        header("Location: /");
        
    }
    else
    {
        // else render form
        render("sell_form.php", ["title" => "Quote"]);
    }

?>
