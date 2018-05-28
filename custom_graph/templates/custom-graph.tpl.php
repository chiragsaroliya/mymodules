
<div class='choices'>
  <ol>
    <?php foreach($choices as $i => $choice): ?>
      <li><?php print $choice; ?></li>
    <?php endforeach; ?>
  </ol>
</div>
<div class="graph">
  <?php print $graph; ?>
</div>
<div class="choices-withcolor">
  <ol>
  <?php $i = 0; foreach($choices as $key => $choice): ?>
    <div>
	  <span class="empty_fill" style="background-color:<?php print $colors[$i] ?>">
	  </span>
	  <li><?php print substr($choice, 0, 6); $i++; ?></li>
	</div>
  <?php endforeach; ?>
  
  </ol>
</div>
