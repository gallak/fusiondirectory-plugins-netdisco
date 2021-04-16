<?php

require_once("netdisco/restclient.php");

class netdisco_server {
  private $server_token = "";
  private $server_url = "";
  private $server_uri = "/api/v1";
  private $username = "";
  private $password = "";
  private $obj = "/object/device";

  // constructueur
  public function __construct($server, $username, $password){
    $this->server_url=$server;
    $this->username=$username;
    $this->password=$password;

  }

  public function doLogin() {

    $options= array( "username" => $this->username, "password" => $this->password, 'base_url' => $this->server_url);
    $param = array();
    $headers = array('Accept' => 'application/json');
    $apiClient = new RestClient($options);
    $result = $apiClient->post('/login',$param,$headers);
    if ( $result->info->http_code == "200" ){
        $this->server_token = $result->decode_response()->api_key;
    }else{
        print( "Error while getting token" );
    }
  }

  private function getRemoteObjects($request){
    $param = array();
    $headers = array('Accept' => 'application/json', 'Authorization' => $this->server_token);
    $apiClient = new RestClient();
    $apiClient->set_option('headers', $headers);
    $apiClient->set_option('base_url',$this->server_url.$this->server_uri);

    $result = $apiClient->get('/object/'.$request);
    if ( $result->info->http_code == "200" ){
        return($result->decode_response());
    }else{
        print( "Error while Remote Objects" );
    }
}


   public function getDevicePorts($ip=''){
       return($this->getRemoteObjects('device/'.$ip.'/ports'));
   }


   public function getSearchNode($criteria){
       return($this->getRemoteSearch('node',$criteria));
   }

   public function getSearchDevice($criteria){
       return($this->getRemoteSearch('device',$criteria));
   }




  private function getRemoteReport(){
  }

  private function getRemoteSearch($type='',$criteria=''){

    $headers = array('Accept' => 'application/json', 'Authorization' => $this->server_token);
    $apiClient = new RestClient();
    $apiClient->set_option('headers', $headers);
    $apiClient->set_option('parameters', $criteria);
    $apiClient->set_option('base_url',$this->server_url.$this->server_uri);

    $result = $apiClient->get('/search/'.$type);

//    var_dump($result);
//    var_dump($this->server_token);
    if ( $result->info->http_code == "200" ){
        return($result->decode_response());
    }else{
        print( "Error while Remote Objects" );
    }
  }

}

?>



