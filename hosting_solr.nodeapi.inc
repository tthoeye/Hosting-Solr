<?php

/** 
 * Implementation of hook_insert()
 */
function hosting_solr_insert($node) {

  if ($node->type == 'site') {
    if ($node->hosting_solr_server) {
      db_query("INSERT INTO {hosting_solr_sites} (vid, nid, hosting_solr_server) VALUES (%d, %d, '%s')", $node->vid, $node->nid, $node->hosting_solr_server);   
    }
  }
  
}

/**
 * Implementation of hook_update()
 */
function hosting_solr_update($node) {
  db_query("UPDATE {hosting_solr_sites} SET hosting_solr_server = '%s'", $node->solr_hosting_server);
}

/**
 * Implementation of hook_delete()
 */
function hosting_solr_delete($node) {
  db_query("DELETE FROM {hosting_solr_sites} WHERE nid=%d", $node->nid);
}

/**
 * Implementation of hook_delete_revision()
 */
function hosting_solr_delete_revision($node) {
  db_query("DELETE FROM {hosting_solr_sites} WHERE vid=%d", $node->vid);
}

/** 
 * Implements hook_nodeapi
 */
function hosting_solr_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL) {
  if ($node->type == 'site') {
    switch ($op) {
      case 'presave':
        hosting_solr_presave($node);
        break;
      case 'insert':
        hosting_solr_insert($node);
        break;
      case 'update':
        hosting_solr_update($node);
        break;
      case 'delete' :
        hosting_solr_delete($node);
        break;
      case 'delete revision':
        hosting_solr_delete_revision(node);
        break;
      case 'validate' :
        // @TODO: validate?
        break;
      case 'load':
        $additions['hosting_solr_server'] = db_result(db_query("SELECT hosting_solr_server FROM {hosting_solr_sites} WHERE vid=%d", $node->vid));
        return $additions;
        break;
    }
  }
}
