<?php

/**
 *  @file
 *  Forms and functions for aws_cloudfront_cdn module.
 */
require_once('libraries/cloudfront/CloudFront.php');

/**
 *  Callback function for setting AWS keys
 *
 *  @return Array
 */
function aws_cloudfront_key_settings() {
  $form['aws_cloudfront_cdn']['aws_key'] = array(
    '#type' => 'textfield',
    '#title' => t('AWS Key'),
    '#default_value' => variable_get('aws_key',''),
  );
  $form['aws_cloudfront_cdn']['aws_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('AWS Secret Key'),
    '#default_value' => variable_get('aws_secret_key',''),
  );
  $form['aws_cloudfront_cdn']['aws_distribution_id'] = array(
    '#type' => 'textfield',
    '#title' => t('AWS Distribution ID'),
    '#default_value' => variable_get('aws_distribution_id',''),
  );
  return system_settings_form($form);
}
/**
 *  @return Array
 */
function aws_cloudfront_cdn_settings() {
  $form = array();
  $form['aws_cloudfront_cdn'] = array(
    '#type' => 'fieldset',
    '#title' => t('AWS CloudFront CDN Settings'),
    '#collapsible' => false,
    '#collapsed' => false,
    '#tree' => false,
  );
  $form['aws_cloudfront_cdn']['distribution'] = array(
    '#type' => 'fieldset',
    '#title' => t('Distribution Information'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#description' => t('All Distributions for the current CloudFront Client'),
  );
  $form['aws_cloudfront_cdn']['distribution']['distributions'] = array(
    '#type' => 'markup',
    '#markup' => _aws_cloudfront_cdn_get_distributions(),
  );
  $form['aws_cloudfront_cdn']['invalidation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Invalidation Information'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#description' => t('Current CloudFront Distribution Invalidations'),
  );
  $form['aws_cloudfront_cdn']['invalidation']['invalidations'] = array(
    '#type' => 'markup',
    '#markup' => _aws_cloudfront_cdn_list_invalidations(),
  );
  $form['aws_cloudfront_cdn']['updated_nodes'] = array(
    '#type' => 'fieldset',
    '#markup' => '',
    '#title' => t('Node Status'),
    '#description' => t('Nodes that have been updated since last CloudFront invalidation.'),
  );
  $form['aws_cloudfront_cdn']['updated_nodes']['nodes'] = array(
    '#type' => 'markup',
    '#markup' => _aws_cloudfront_cdn_get_updated_nodes(),
  );
  $form['aws_cloudfront_cdn']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Invalidate Cache',
  );
  $form['#submit'][] = "_aws_cloudfront_cdn_invalidate";

	return $form;
}

/**
 *  Callback function to return either a table of invalidation information
 *  about the current distribution, or a string containing an error.
 *
 *  @return Array or String
 */
function _aws_cloudfront_cdn_list_invalidations() {
  $cf = new CloudFront();
  if(is_array($cf->retrieveInvalidations())) {
    $return = theme("aws_cloudfront_cdn_invalidations", 
    array(
      "invalidations" => $cf->retrieveInvalidations())
    );
  }
  else {
    $return = $cf->retrieveInvalidations();
  }
  return $return;
}

/**
 *  Callback function to return either a table of distribution information
 *  about the current domain, or a string containing an error.
 *
 *  @return Array or String
 */
function _aws_cloudfront_cdn_get_distributions() {
  $cf = new CloudFront();
  if(is_array($cf->retrieveDistributions())) {
    $return = theme("aws_cloudfront_cdn_distributions", array(
    "distributions" => $cf->retrieveDistributions()));
  }
  else {
    $return = $cf->retrieveDistributions();
  }
  return $return;
}

/**
 *  Callback function to invalidate CloudFront cache on the current
 *  environment for paths pending invalidation.
 *  
 *  If there are no pending paths, then clicking the button will not
 *  trigger an invalidation request.
 */
function _aws_cloudfront_cdn_invalidate() {
  $last_invalidation = variable_get("cloudfront_last_invalidation",time());

  if(!isset($last_invalidation)) {
    variable_set("cloudfront_last_invalidation",time());
  }
  $cf = new CloudFront();
  $nodes = _aws_cloudfront_cdn_get_invalidation_paths();
  if(count($nodes) > 0) {
    $paths = array();
    $host = (!empty($_SERVER['HTTPS']) ? "https://" : "http://") . $_SERVER['HTTP_HOST'];
    foreach($nodes as $file) {
      $paths[] = str_replace($host,"",$file);
    }

    $cf->createInvalidation($paths);
  }

}
/**
 *  Callback function to get the paths to files that have been updated
 *  since last invalidation request.
 *
 *  @return Array
 */
function _aws_cloudfront_cdn_get_invalidation_paths() {
  $paths = array();
  $managed_files = _aws_cloudfront_cdn_get_updated_files();
  if(count($managed_files) > 0) {
    foreach($managed_files as $managed_file) {
      $paths[] = str_replace("https://".$_SERVER['SERVER_NAME'],'',file_create_url($managed_file->uri));
    }
  }
  return $paths;
}

/**
 *  Callback function to query file managed table for files
 *  that have been updated since last invalidation.
 *
 *  @return Array
 */
function _aws_cloudfront_cdn_get_updated_nodes() {
  return theme("aws_cloudfront_cdn_invalidated_nodes", array(
    "managed_files" => _aws_cloudfront_cdn_get_updated_files(),
  ));
}

/**
 *  Helper function to get all managed files that have been updated
 *  since the last invalidation request.
 *
 *  @return Array
 */
function _aws_cloudfront_cdn_get_updated_files() {
  $last_invalidation = variable_get('cloudfront_last_invalidation',time());

  $managed_files = db_select('file_managed','f')
    ->fields('f')
    ->condition('timestamp',$last_invalidation,'>=')
    ->condition('status',1)
    ->condition('type','video','<>')
    ->execute()
    ->fetchAll();

  return $managed_files;
}