<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // make sure the user provided a stock ticker
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a stock symbol.");
            exit;
        }
        
        // make sure the user provided how many stocks are wanted
        if (empty($_POST["shares"]))
        {
            apologize("You must provide how many you want to buy.");
            exit;
        }
        
        // make sure the user indicated a positive, whole number
        if (preg_match("/^\d+$/", htmlspecialchars($_POST["shares"])) == false)
        {
            apologize("We can only sell positive, whole stocks");
            exit;
        }
        
        // lookup stocks for symbol
        $s = lookup(htmlspecialchars($_POST["symbol"]));
       
        // check if the stock is valid
        if ($s === FALSE)
        {
            apologize("That is not a valid stock");
            exit;
        }
        
        // find out how much cash the user has
        $cashs = query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
        $cash = $cashs[0]["cash"]; 
        
        // calculate the value of the amount of stock the user wants
        $valuedesired =  $s["price"]* htmlspecialchars($_POST["shares"]); 
        
        // check if the user has enough money.
        if ($valuedesired > $cash)
        {
            apologize("You do not have enough cash for this transaction");
            exit;
        }
        
        // make sure that it is capitalized
        $symbol = strtoupper(htmlspecialchars($_POST["symbol"]));
        
        // update new amount of cash
        $newcash = query("UPDATE users SET cash = cash - ? WHERE id = ?", $valuedesired, $_SESSION["id"]);
        
        // add to database
        $adder = query("INSERT INTO stocks (id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)", $_SESSION["id"], $symbol, htmlspecialchars($_POST["shares"]));
        
        // update the history database
        $updatehistory = query("INSERT INTO history (type, symbol, shares, price, id) VALUES(?, ?, ?, ?, ?)", 'BUY', $symbol, htmlspecialchars($_POST["shares"]), $s["price"], $_SESSION["id"]);
        header("Location: /");
    }
    else
    {
        // else render form
        render("buy_form.php", ["title" => "Quote"]);
    }

?>
