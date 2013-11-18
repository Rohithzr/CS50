<?php

    // configuration
    require("../includes/config.php"); 

    // query the server
    $rows = query("SELECT type, timestamp, symbol, shares, price FROM history WHERE id = ?", $_SESSION["id"]);
    
 
    
    
    // render portfolio
    render("history_table.php", ["title" => "Portfolio", "rows" => $rows]);
    
?>
