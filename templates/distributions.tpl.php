<table>
  <thead>
    <th>Distribution ID</th>
    <th>Domain Name</th>
    <th>Last Modified</th>
    <th>Origin</th>
  </thead>
  <tbody>
    <?php foreach($distributions as $distribution): ?>
      <tr>
        <td><?php print $distribution['Id'];?></td>
        <td><?php print $distribution['DomainName'];?></td>
        <td><?php print $distribution['LastModifiedTime'];?></td>
        <td><?php print $distribution['Origins']['Items'][0]['DomainName'];?></td>

      </tr>
   <?php endforeach;?>
  </tbody>
</table>