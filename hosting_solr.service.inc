<?php



class hostingService_search extends hostingService {
  public $service = 'search';
  public $ssl_enabled = FALSE;
}


class hostingService_search_public extends hostingService_search {
  protected $has_port = TRUE;
  
  function default_port() {
    return 8080;
  }

  function update() {
    parent::update();
  }
  
  public function context_options($task_type, $ref_type, &$task) {
    parent::context_options($task_type, $ref_type, $task);
  }
}


class hostingService_search_solr extends hostingService_search_public {
  public $type = 'solr';

  protected $has_restart_cmd = FALSE;
  
  /**
   * Form trblz: http://drupal.org/node/1016890
   */
  function form(&$form) {
    $form = parent::form($form);
    $form['port'] = array(
      '#type' => 'textfield',
      '#required' => $this->available,
      '#title' => t('Port'),
      '#description' => t('The Port of the Solr server.'),
      '#size' => 40,
      '#default_value' => $this->port,
      '#maxlength' => 255,
      '#weight' => 5,
    );
    $form['solr_path'] = array(
      '#type' => 'textfield',
      '#required' => $this->available,
      '#title' => t('Path'),
      '#description' => t('The path, relative to the hostname, of the Solr server instance'),
      '#size' => 40,
      '#default_value' => $this->solr_path,
      '#maxlength' => 64,
      '#weight' => 6,
    );
    $form['solr_enabled'] = array(
      '#type' => 'checkbox',
      '#required' => $this->available,
      '#title' => t('Enabled'),
      '#description' => t('Whether or not this server is enabled'),
      '#default_value' => $this->solr_enabled,
    );
  }    

  function insert() {
    parent::insert();
    $port = intval($this->solr_port);
    db_query("INSERT INTO {hosting_solr_server} (`vid`, `nid`, `solr_url`, `solr_path`, `solr_enabled`) VALUES (%d, %d, '%s', '%s', %d)", 
      $this->server->vid, $this->server->nid, $this->solr_url, $this->solr_path, $this->solr_enabled);
  }

  function update() {
    // only do the parent's update routine.
    parent::delete_revision();
    parent::insert();
  }

  function delete_revision() {
    parent::delete_revision();
    db_query('DELETE FROM {hosting_solr_server} WHERE vid = %d', $this->server->vid);
  }

  function delete() {
    parent::delete();
    db_query('DELETE FROM {hosting_solr_server} WHERE nid = %d', $this->server->nid);
  }


  function load() {
    parent::load();
    $this->mergeData('SELECT `solr_url`, `solr_path`, `solr_enabled` FROM {hosting_solr_server} WHERE vid = %d', $this->server->vid);
  }


  /**
   * @TODO: Filter XSS
   */
  function view(&$render) {
    parent::view($render);
  }

  public function context_options($task_type, $ref_type, &$task) {
    parent::context_options($task_type, $ref_type, $task);
    
    // Provide context_options for verification and writing out to an alias
    $task->context_options['master_db'] = 'mysql' . '://' . $this->user . ':' . $this->passwd . '@' . $this->server->title;
    $task->contect_options['solr'] = 'yes';
  }

  public function context_import($context) {
    parent::context_import($context);

    $matches = array();
    preg_match("+^http://(.*):(.*)@.*$+", stripslashes($context->master_db), $matches);
    $this->user = $matches[1];
    $this->passwd = $matches[2];
  }

}
