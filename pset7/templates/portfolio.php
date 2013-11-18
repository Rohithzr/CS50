<div class="maintable">
<table class="table table-striped">
    <!-- Column headings for the table -->
    <thead>
        <tr>
            <th>Company</th>
            <th>Symbol</th>
            <th>Shares</th>
            <th>Price</th>
            <th>Total</th>
        </tr>
    </thead>
  
  

    <?php
    
        // query the database to get the amount of cash the user has
        // format the number to 2 decimal places
        $balances = query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
        $balance = $balances[0]["cash"];            
        $balance = number_format($balance, 2);
        
        // loop over each of the stocks
        foreach ($positions as $position)
        {
            // determine the stock value and format it
            $value = $position["price"] * $position["shares"];
            $value = number_format($value, 2);
            
            // format the price
            $position["price"] =  number_format($position["price"], 2);
           
            // print all of the elements of a row
            print("<tr>");
            print("<td>{$position["name"]}</td>");
            print("<td>{$position["symbol"]}</td>");
            print("<td>{$position["shares"]}</td>");
            print("<td>$ {$position["price"]}</td>");
            print("<td>$ {$value}</td>");

            print("</tr>");
        }
        
        print("<tr>"); ?>

        <td colspan="4" style="text-align:left"><b>Cash</b></td>
        <?php
        
        // print the final cash balalnce
        print("<td><b>$ {$balance}</b></td>");
        print("</tr>");
        
    ?>
</table>
</div>
<div>
    <a href="logout.php">Log Out</a>
</div>
