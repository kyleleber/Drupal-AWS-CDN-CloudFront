<table>
  <thead>
    <th>Invalidation #</th>
    <th>Invalidation ID</th>
    <th>Time Created</th>
    <th>Status</th>
  </thead>
  <tbody>
    <?php foreach($invalidations as $index => $invalidation): ?>
      <tr>
        <td><?php print $index;?></td>
        <td><?php print $invalidation['Id'];?></td>
        <td><?php print $invalidation['CreateTime'];?></td>
        <td><?php print $invalidation['Status'];?></td>
      </tr>
   <?php endforeach;?>
  </tbody>
</table>