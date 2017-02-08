<?php
/**
 *  CloudFront is a class used to integrate with an AWS subscription using CloudFrontClient.
 *
 *  @author Kyle Leber <kjl16@psu.edu>
 *  @see http://docs.aws.amazon.com/aws-sdk-php/v2/api/class-Aws.CloudFront.CloudFrontClient.html
 */
 
require_once('aws.phar');
use Aws\CloudFront\CloudFrontClient;

Class CloudFront {
  
  /**
   *  Initialize the CloudFrontClient factory object and use the
   *  variables stored in global $conf to set the keys and proxy.
   */
  function __construct() {
    global $conf;
    $this->_key = variable_get('aws_key');
    $this->_secret_key = variable_get('aws_secret_key');
    $this->_distribution_id = variable_get('aws_distribution_id');
   
    $this->_Client = CloudFrontClient::factory(array(
      'key' => $this->_key,
      'secret'=> $this->_secret_key,
    ));
  }
  
  /**
   *  Retrieve a list of distributions from AWS
   *
   *  @return 
   *   - Success: Array of Distribution Information
   *   - Failure: String
   */
  public function retrieveDistributions() {
    try {
      $distributions =  $this->_Client->listDistributions();
    }
    catch(Exception $e) {
      return $e->getMessage();
    }
    
    return $distributions['Items'];
  }
  
  /**
   *  Retrieve a list of invalidations from AWS.
   *  The invalidations are based on the CDN Distribution ID, which
   *  is managed in the global $conf variable.
   *
   *  @return
   *   - Success: Array of Invalidation Information
   *   - Failure: String
   */
  public function retrieveInvalidations(){
    global $conf;
    try {
      $validations = $this->_Client->listInvalidations(array('DistributionId'=> $this->_distribution_id));
    }
    catch(Exception $e) {
      return $e->getMessage();
    }
    
    return $validations['Items'];
  }
  
  /**
   *  Create an invalidation request to AWS and if successful,
   *  update the cloudfront_last_invalidation variable to the current
   *  timestamp.
   *
   *  @param $paths Array of Strings
   */
  public function createInvalidation($paths) {
    global $conf;
    try {
      $this->_Client->createInvalidation(
        array(
          'DistributionId' => $this->_distribution_id,
          'Paths' => array(
            'Quantity' => count($paths),
            'Items' => $paths,
          ),
          'CallerReference' => time()
        )
      );
      drupal_set_message("Invalidation Request has been sent. Check the pending invalidations to see its progress.");
      variable_set("cloudfront_last_invalidation",time());
    }
    catch(Exception $e) {
      drupal_set_message($e->getMessage(),'warning');
      watchdog('cloudfront',$e->getMessage(),array(),WATCHDOG_WARNING);
    }
  }
}

?>
