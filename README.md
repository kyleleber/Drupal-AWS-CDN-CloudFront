File Contents
---------------------
   
 * Introduction
 * Requirements
 * Drupal Module Dependencies
 * Installation
 * Configuration


INTRODUCTION
------------

Current Maintainer: Kyle Leber <kyleleber2014@gmail.com>

Amazon CloudFront is a global content delivery network (CDN) service that accelerates delivery of your websites, APIs, video content or other web assets. It integrates with other Amazon Web Services products to give developers and businesses an easy way to accelerate content to end users with no minimum usage commitments.
**https://aws.amazon.com/cloudfront/

This module is used to create invalidation requests to AWS CloudFront to remove objects from the edge caches before those objects expire and allow the new content to be cached instead. 

When a managed file in Drupal is updated, the entity will appear in administrative backend of this module. Clicking on the invalidate cache button will trigger an invalidation request to AWS CloudFront using the keys provided as well as the distribution id.

You can view information about the last 100 invalidation requests as well as view the current distributions available within your AWS subscription.


REQUIREMENTS
------------

* An active AWS CloudFront license with provided keys
* A Drupal 7 installation with the following required module dependencies
** media
** file_entity

*** NOTE: These modules will automatically download and install when this module
          is enabled.
          
          
DRUPAL MODULE DEPENDENCIES
--------------------------
* https://www.drupal.org/project/media
* https://www.drupal.org/project/file_entity


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
   

CONFIGURATION
-------------

* Navigate to Configuration -> System -> AWS CloudFront
* Select Key Configuration and enter the keys provided by AWS
* Click 'Save configuration'
* Navigate to Configuration -> System -> AWS CloudFront
* Select AWS Cloudfront to view and manage CloudFront distributions and create invalidation requests
