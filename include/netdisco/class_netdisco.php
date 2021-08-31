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

   public function getDeviceInfo($ip=''){
       return($this->getRemoteObjects('device/'.$ip));
   }

   public function getDevicePorts($ip=''){
       return($this->getRemoteObjects('device/'.$ip.'/ports'));
   }

   public function getDevicePortVlans($ip='',$port=''){
       return($this->getRemoteObjects('device/'.$ip.'/port/'.$port.'/vlans'));
   }

   public function getDevicePortPower($ip='',$port=''){
       return($this->getRemoteObjects('device/'.$ip.'/port/'.$port.'/power'));
   }

   public function getDevicePortDetails($ip='',$port=''){
       return($this->getRemoteObjects('device/'.$ip.'/port/'.$port));
   }


   public function getSearchNode($criteria){
       return($this->getRemoteSearch('node',$criteria));
   }

   public function getSearchDevice($criteria){
       return($this->getRemoteSearch('device',$criteria));
   }

/*0:{
"percent":6
"subnet":"10.69.80.0/24"
"active":15
"subnet_size":256
}*/
   public function getReportIpSubnets($criteria){
       return($this->getRemoteReport('ip/subnets',$criteria));
   }


  private function getRemoteReport($type='',$criteria=''){
    $headers = array('Accept' => 'application/json', 'Authorization' => $this->server_token);
    $apiClient = new RestClient();
    $apiClient->set_option('headers', $headers);
    $apiClient->set_option('parameters', $criteria);
    $apiClient->set_option('base_url',$this->server_url.$this->server_uri);

    $result = $apiClient->get('/report/'.$type);

    if ( $result->info->http_code == "200" ){
        return($result->decode_response());
    }else{
        print( "Error while Remote reports" );
    }
  }



  private function getRemoteSearch($type='',$criteria=''){

    $headers = array('Accept' => 'application/json', 'Authorization' => $this->server_token);
    $apiClient = new RestClient();
    $apiClient->set_option('headers', $headers);
    $apiClient->set_option('parameters', $criteria);
    $apiClient->set_option('base_url',$this->server_url.$this->server_uri);

    $result = $apiClient->get('/search/'.$type);

    if ( $result->info->http_code == "200" ){
        return($result->decode_response());
    }else{
        print( "Error while Remote Objects" );
    }
  }

}

?>



