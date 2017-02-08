<table>
  <thead>
    <th>File ID</th>
    <th>File Name</th>
    <th>Path</th>
    <th>Last Updated</th>
    <th>Type</th>
  </thead>
  <tbody>
    <?php foreach($managed_files as $file): ?>
      <tr>
        <td><?php print $file->fid;?></td>
        <td><?php print $file->filename;?></td>
        <?php
          $host = (!empty($_SERVER['HTTPS']) ? "https://" : "http://") . $_SERVER['HTTP_HOST'];
        ?>
        <td><?php print str_replace($host,"",file_create_url($file->uri));?></td>
        <td><?php print $file->timestamp;?></td>
        <td><?php print $file->type;?></td>
      </tr>
   <?php endforeach;?>
  </tbody>
</table>