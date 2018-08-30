<?php
/**
*   based on
*   drupal.org/node/1269066#comment-4988994     
*   please look there!
*/
abstract class MYSITE_Basic_Migration	 extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    ini_set('auto_detect_line_endings', TRUE);
  }
}

class MYSITEUserMigration extends MYSITE_Basic_Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    
    $this->description = t('Import an MYSITE Users from CSV-file (User Account fields)');
    $columns = array(
		0 => array('id', t('id')),
		1 => array('idMemberApplication', t('idMemberApplication')),
		2 => array('username', t('username')),
		3 => array('password', t('password')),
		4 => array('email', t('email')),
		5 => array('passwordQuestion', t('passwordQuestion')),
		6 => array('passwordAnswer', t('passwordAnswer')),
		7 => array('dateLastActivity', t('dateLastActivity')),
		8 => array('dateLastLogin', t('dateLastLogin')),
		9 => array('dateLastPasswordChanged', t('dateLastPasswordChanged')),
		10 => array('isApproved', t('isApproved')),
		11 => array('isOnLine', t('isOnLine')),
		12 => array('isLockedOut', t('isLockedOut')),
		13 => array('lastLockedOutDate', t('lastLockedOutDate')),
		14 => array('failedPasswordAttemptCount', t('failedPasswordAttemptCount')),
		15 => array('failedPasswordAttemptWindowStart', t('failedPasswordAttemptWindowStart')),
		16 => array('failedPasswordAnswerAttemptCount', t('failedPasswordAnswerAttemptCount')),
		17 => array('failedPasswordAnswerAttemptWindowStart', t('failedPasswordAnswerAttemptWindowStart')),
		18 => array('comment', t('comment')),
		19 => array('dateCreated', t('dateCreated')),
		20 => array('dateUpdated', t('dateUpdated')),
		21 => array('DateRegistration', t('DateRegistration')),
		22 => array('NameFirst', t('NameFirst')),
		23 => array('DateQuit', t('DateQuit')),
		24 => array('DateOfBirth', t('DateOfBirth')),
		25 => array('Gender', t('Gender')),
		26 => array('NameLast', t('NameLast')),
		27 => array('DateTreatmentStart', t('DateTreatmentStart')),
		28 => array('FollowUpAppointmentDate', t('FollowUpAppointmentDate')),
		29 => array('TermsAndConditions', t('TermsAndConditions')),
		30 => array('Barcode', t('Barcode')),
		31 => array('PhoneMobile', t('PhoneMobile')),
		32 => array('SmsTime', t('SmsTime')),
		33 => array('ImportedFromGuildcare', t('ImportedFromGuildcare')),
		34 => array('State', t('State')),
		35 => array('HideWelcomePopup', t('HideWelcomePopup')),
		36 => array('OptOutSMS', t('OptOutSMS')),
		37 => array('Postcode', t('Postcode')),
		38 => array('OptOutEmail', t('OptOutEmail')),
		39 => array('Nickname', t('Nickname')),
		40 => array('YearsSmoking', t('YearsSmoking')),
		41 => array('CigarettesEachDay', t('CigarettesEachDay')),
		42 => array('TimesQuitSmoking', t('TimesQuitSmoking')),
		43 => array('TriedChampix', t('TriedChampix')),
		44 => array('PrimaryTriggers', t('PrimaryTriggers')),
		45 => array('Fears', t('Fears')),
		46 => array('ReasonForQuitting', t('ReasonForQuitting')),
		47 => array('ImportSource', t('ImportSource')),
		48 => array('ImportProgram', t('ImportProgram'))
    );     
    $csv = DRUPAL_ROOT . '/' . drupal_get_path('module', 'mysite_migrate') . '/data_sources/file.csv';
    
    $this->source = new MigrateSourceCSV($csv, $columns, 
			array(
				'header_rows' => 1,
				'embedded_newlines' => TRUE,
			  )
		   );
		   
    $this->destination = new MigrateDestinationUser();    
    $this->map = new MigrateSQLMap($this->machineName,
		  array(
			'id' => array(
			  'type' => 'int',
			  'unsigned' => TRUE,
			  'not null' => TRUE,
			),
		  ),
        MigrateDestinationUser::getKeySchema()
    );

    // Mapped fields
    $this->addFieldMapping('name', 'username')
		 ->dedupe('users', 'name');
    $this->addFieldMapping('mail', 'email')
		 ->dedupe('users', 'mail');    
    $this->addFieldMapping('pass', 'password');    
    $this->addFieldMapping('created')
		 ->defaultValue(time());
    $this->addFieldMapping('init', 'email');          
    $this->addFieldMapping('is_new')
      ->defaultValue(TRUE);      
    $this->addFieldMapping('roles')
      ->defaultValue(2);     
    $this->addFieldMapping('access')
      ->defaultValue(0);      
    $this->addFieldMapping('login')
      ->defaultValue(0);      
    $this->addFieldMapping('status')
      ->defaultValue(1);
    $this->addFieldMapping('field_affiliate')
      ->defaultValue('migrated');
  }
  
  /**
   * Method to alter data before import.
   * This will be triggered for every record.
   */
  public function prepareRow($row) {	  
	//change the options of csv (Once,Twice,More than 15 times,11 - 15 times,6 - 10 times,3 times) with our new option value 
    if ($row->email) {
		//$row->email = "temp".$row->email;
		$row->email = $row->email;
    }
    if ($row->username) {
		//$row->username = "temp".$row->username;
		$row->username = $row->username;
    }
  }  
  /**
   * Method to update data on every row insert/update complete.
   *
   * This will be triggered for every record.
   */
  public function complete($account, $row) {
    // User save tries to convert the password into hash.
    // Plain password save https://www.drupal.org/node/1349758.
    db_update('users')
    ->fields(array('pass' => '$N$'.$row->password))
    ->condition('uid', $account->uid)
    ->execute();
  }  
  
}


class MYSITE_Step1_UserProfileMigration extends MYSITE_Basic_Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    
    $this->description = t('Import an CSV-file with Profile2-fields ("Step1 Profile fields")');
    $columns = array(
		0 => array('id', t('id')),
		1 => array('idMemberApplication', t('idMemberApplication')),
		2 => array('username', t('username')),
		3 => array('password', t('password')),
		4 => array('email', t('email')),
		5 => array('passwordQuestion', t('passwordQuestion')),
		6 => array('passwordAnswer', t('passwordAnswer')),
		7 => array('dateLastActivity', t('dateLastActivity')),
		8 => array('dateLastLogin', t('dateLastLogin')),
		9 => array('dateLastPasswordChanged', t('dateLastPasswordChanged')),
		10 => array('isApproved', t('isApproved')),
		11 => array('isOnLine', t('isOnLine')),
		12 => array('isLockedOut', t('isLockedOut')),
		13 => array('lastLockedOutDate', t('lastLockedOutDate')),
		14 => array('failedPasswordAttemptCount', t('failedPasswordAttemptCount')),
		15 => array('failedPasswordAttemptWindowStart', t('failedPasswordAttemptWindowStart')),
		16 => array('failedPasswordAnswerAttemptCount', t('failedPasswordAnswerAttemptCount')),
		17 => array('failedPasswordAnswerAttemptWindowStart', t('failedPasswordAnswerAttemptWindowStart')),
		18 => array('comment', t('comment')),
		19 => array('dateCreated', t('dateCreated')),
		20 => array('dateUpdated', t('dateUpdated')),
		21 => array('DateRegistration', t('DateRegistration')),
		22 => array('NameFirst', t('NameFirst')),
		23 => array('DateQuit', t('DateQuit')),
		24 => array('DateOfBirth', t('DateOfBirth')),
		25 => array('Gender', t('Gender')),
		26 => array('NameLast', t('NameLast')),
		27 => array('DateTreatmentStart', t('DateTreatmentStart')),
		28 => array('FollowUpAppointmentDate', t('FollowUpAppointmentDate')),
		29 => array('TermsAndConditions', t('TermsAndConditions')),
		30 => array('Barcode', t('Barcode')),
		31 => array('PhoneMobile', t('PhoneMobile')),
		32 => array('SmsTime', t('SmsTime')),
		33 => array('ImportedFromGuildcare', t('ImportedFromGuildcare')),
		34 => array('State', t('State')),
		35 => array('HideWelcomePopup', t('HideWelcomePopup')),
		36 => array('OptOutSMS', t('OptOutSMS')),
		37 => array('Postcode', t('Postcode')),
		38 => array('OptOutEmail', t('OptOutEmail')),
		39 => array('Nickname', t('Nickname')),
		40 => array('YearsSmoking', t('YearsSmoking')),
		41 => array('CigarettesEachDay', t('CigarettesEachDay')),
		42 => array('TimesQuitSmoking', t('TimesQuitSmoking')),
		43 => array('TriedChampix', t('TriedChampix')),
		44 => array('PrimaryTriggers', t('PrimaryTriggers')),
		45 => array('Fears', t('Fears')),
		46 => array('ReasonForQuitting', t('ReasonForQuitting')),
		47 => array('ImportSource', t('ImportSource')),
		48 => array('ImportProgram', t('ImportProgram'))
    );    
    $csv = DRUPAL_ROOT . '/' . drupal_get_path('module', 'mysite_migrate') . '/data_sources/file.csv';
    $this->source = new MigrateSourceCSV( $csv, $columns, 
						array(
							'header_rows' => 1,
							'embedded_newlines' => TRUE,
						)
					);    
    
    $this->dependencies = array('MYSITEUserMigration');
    
    $this->destination = new MigrateDestinationProfile2('registration');  
    
    $this->map = new MigrateSQLMap($this->machineName,
		  array(
			'id' => array(
			  'type' => 'int',
			  'unsigned' => TRUE,
			  'not null' => TRUE,
			),
		  ),
        MigrateDestinationProfile2::getKeySchema()
    );

    // Mapped fields
	$this->addFieldMapping('uid', 'id')
         ->sourceMigration('MYSITEUserMigration')  // If your user migration class was named 'MyUserMigration', the string is 'MyUser'
         ->description(t('The assignment of profile2-items to the respective user'));	
    $this->addFieldMapping('field_first_name', 'NameFirst')
		 ->description(t('The First name.'));
    $this->addFieldMapping('field_last_name', 'NameLast')
		 ->description(t('The Last name.'));
    $this->addFieldMapping('field_gender', 'Gender')
		 ->description(t('Gender male/female'));    
    $this->addFieldMapping('field_dob', 'DateOfBirth')
//    $this->addFieldMapping('field_dob')
//		 ->defaultValue('2010-01-01')
		 ->description(t('DOB in the format of YYYY-MM-DD H:m:s'));
    $this->addFieldMapping('field_state', 'State');		 
    $this->addFieldMapping('field_start_medication_date', 'DateTreatmentStart')
		 ->description(t('Start medication date in the format of YYYY-MM-DD H:m:s'));	    		     
    $this->addFieldMapping('field_quit_smoking_date', 'DateQuit')
		 ->description(t('Quit smoking date in the format of YYYY-MM-DD H:m:s'));	    		 
    $this->addFieldMapping('field_preferred_method_of_contac')
		 ->defaultValue('email');
    $this->addFieldMapping('field_postcode', 'Postcode');		 
    $this->addFieldMapping('field_mobile_number', 'PhoneMobile');
//		 ->defaultValue('9876543210');    
    $this->addFieldMapping('field_email_id');
//		 ->defaultValue('test.test');				
    $this->addFieldMapping('field_domain_name');
//    	 ->defaultValue('test.com');				
  }
  
  /**
   * Method to alter data before import.
   * This will be triggered for every record.
   */
  public function prepareRow($row) {
	
    if ($row->DateQuit) {
		$row->DateQuit = $this->formatdate($row->DateQuit);		
    }
    if ($row->Postcode) {
		$Postcode = preg_replace("/[^a-zA-Z0-9]/", "", $row->Postcode);
		if (ctype_digit($Postcode) && strlen($Postcode) == "4") {
			$row->Postcode = $Postcode;			
		} else {
			$row->Postcode = "";
		}
    }    
    if ($row->State) {
		$states = array( 'ACT','NSW','NT','QLD','SA','TAS','VIC','WA');
		$str = strtoupper($row->State);
		if (in_array($str, $states)) {
			$row->State = $str;
		} else {
			$row->State = "";
		}
    }    
    if ($row->Gender) {
		$gender_options = array('male', 'female');
		if (in_array(strtolower($row->Gender), $gender_options)) {
			$gender = strtolower($row->Gender);
			$row->Gender = $gender;
		} else {
			$row->Gender = "";
		}
    }
    if ($row->DateOfBirth) {	
		$row->DateOfBirth = $this->formatdate($row->DateOfBirth);
    }
    if ($row->DateTreatmentStart) {	
		$row->DateTreatmentStart = $this->formatdate($row->DateTreatmentStart);
    }
    if ($row->PhoneMobile) {
		$PhoneMobile = preg_replace("/[^a-zA-Z0-9]/", "", $row->PhoneMobile);
		if (ctype_digit($PhoneMobile)) {
			if (substr( trim($PhoneMobile ), 0, 2 ) == "61" && strlen( trim($PhoneMobile) ) >= "11") {
				$newphone = substr(trim($PhoneMobile), 2);
				$row->PhoneMobile = $newphone;
			}
			else if (substr( trim($PhoneMobile ), 0, 1 ) == "0" && strlen( trim($PhoneMobile) ) >= "10") {
				$newphone = substr(trim($PhoneMobile), 1);
				$row->PhoneMobile = $newphone;				
			}
			if (!(strlen($row->PhoneMobile) >= "9" && strlen($row->PhoneMobile) <= "11")) {
				$row->PhoneMobile = "";
			}
			//$row->PhoneMobile = '91'.$PhoneMobile;
		} else {
			$row->PhoneMobile = "";
		}
    }    
    return TRUE;
  }
         
  public function prepare(stdClass $account, stdClass $row) { 
	if ($row->email) {
		$email = $row->email;
		$email = explode("@", $email);
		$account->field_email_id['und'][0]['value'] = $email[0];
		$account->field_domain_name['und'][0]['value'] = $email[1];		
	}
	if (!empty($row->PhoneMobile)) {
		$account->field_preferred_method_of_contac['und'][0]['value'] = "both";
		//$account->field_preferred_method_of_contac['und'][0]['value'] = "email";
	}	
  }
  
           
  public function formatdate($date) {
	$dasheddate = str_replace('/', '-', $date);
	$date = date('Y-m-d', strtotime($dasheddate));			
	return $date;
  }    
  
}

class MYSITE_Step2_UserProfileMigration extends MYSITE_Basic_Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    
    $this->description = t('Import an CSV-file with Profile2-fields ("Step2 Profile fields")');
    
    $columns = array(
		0 => array('id', t('id')),
		1 => array('idMemberApplication', t('idMemberApplication')),
		2 => array('username', t('username')),
		3 => array('password', t('password')),
		4 => array('email', t('email')),
		5 => array('passwordQuestion', t('passwordQuestion')),
		6 => array('passwordAnswer', t('passwordAnswer')),
		7 => array('dateLastActivity', t('dateLastActivity')),
		8 => array('dateLastLogin', t('dateLastLogin')),
		9 => array('dateLastPasswordChanged', t('dateLastPasswordChanged')),
		10 => array('isApproved', t('isApproved')),
		11 => array('isOnLine', t('isOnLine')),
		12 => array('isLockedOut', t('isLockedOut')),
		13 => array('lastLockedOutDate', t('lastLockedOutDate')),
		14 => array('failedPasswordAttemptCount', t('failedPasswordAttemptCount')),
		15 => array('failedPasswordAttemptWindowStart', t('failedPasswordAttemptWindowStart')),
		16 => array('failedPasswordAnswerAttemptCount', t('failedPasswordAnswerAttemptCount')),
		17 => array('failedPasswordAnswerAttemptWindowStart', t('failedPasswordAnswerAttemptWindowStart')),
		18 => array('comment', t('comment')),
		19 => array('dateCreated', t('dateCreated')),
		20 => array('dateUpdated', t('dateUpdated')),
		21 => array('DateRegistration', t('DateRegistration')),
		22 => array('NameFirst', t('NameFirst')),
		23 => array('DateQuit', t('DateQuit')),
		24 => array('DateOfBirth', t('DateOfBirth')),
		25 => array('Gender', t('Gender')),
		26 => array('NameLast', t('NameLast')),
		27 => array('DateTreatmentStart', t('DateTreatmentStart')),
		28 => array('FollowUpAppointmentDate', t('FollowUpAppointmentDate')),
		29 => array('TermsAndConditions', t('TermsAndConditions')),
		30 => array('Barcode', t('Barcode')),
		31 => array('PhoneMobile', t('PhoneMobile')),
		32 => array('SmsTime', t('SmsTime')),
		33 => array('ImportedFromGuildcare', t('ImportedFromGuildcare')),
		34 => array('State', t('State')),
		35 => array('HideWelcomePopup', t('HideWelcomePopup')),
		36 => array('OptOutSMS', t('OptOutSMS')),
		37 => array('Postcode', t('Postcode')),
		38 => array('OptOutEmail', t('OptOutEmail')),
		39 => array('Nickname', t('Nickname')),
		40 => array('YearsSmoking', t('YearsSmoking')),
		41 => array('CigarettesEachDay', t('CigarettesEachDay')),
		42 => array('TimesQuitSmoking', t('TimesQuitSmoking')),
		43 => array('TriedChampix', t('TriedChampix')),
		44 => array('PrimaryTriggers', t('PrimaryTriggers')),
		45 => array('Fears', t('Fears')),
		46 => array('ReasonForQuitting', t('ReasonForQuitting')),
		47 => array('ImportSource', t('ImportSource')),
		48 => array('ImportProgram', t('ImportProgram'))   
    );
    
    $csv = DRUPAL_ROOT . '/' . drupal_get_path('module', 'mysite_migrate') . '/data_sources/file.csv';
    $this->source = new MigrateSourceCSV( $csv, $columns, 
						array(
							'header_rows' => 1,
							'embedded_newlines' => TRUE,
						)
					);    
    $this->dependencies = array('MYSITEUserMigration');
    
    $this->destination = new MigrateDestinationProfile2('step_2_registeration');  
    
    $this->map = new MigrateSQLMap($this->machineName,
		  array(
			'id' => array(
			  'type' => 'int',
			  'unsigned' => TRUE,
			  'not null' => TRUE,
			),
		  ),
        MigrateDestinationProfile2::getKeySchema()
    );
	$this->addFieldMapping('uid', 'id')
         ->sourceMigration('MYSITEUserMigration')  // If your user migration class was named 'MyUserMigration', the string is 'MyUser'
         ->description(t('The assignment of profile2-items to the respective user'));
	$this->addFieldMapping('field__which_year_did_you_start_', 'YearsSmoking');       
	$this->addFieldMapping('field_how_many_cigarettes_do_you', 'CigarettesEachDay');       
	$this->addFieldMapping('field_have_you_used_champix_befo', 'TriedChampix');       
	$this->addFieldMapping('field_how_many_times_have_you_tr', 'TimesQuitSmoking');       
	$this->addFieldMapping('field_what_triggers_you_to_smoke', 'PrimaryTriggers');       
	$this->addFieldMapping('field_main_concerns_for_quitting', 'Fears');       
    //~ $this->addUnmigratedSources(array('mail', 'pass'));
  }
  
  /**
   * Method to alter data before import.
   * This will be triggered for every record.
   */
  public function prepareRow($row) {	

    //change the options of csv (Less than a year,11 - 15 years,More than 15,6 - 10 years) with our new option value for field__which_year_did_you_start_
    if ($row->YearsSmoking) {
		$YearsOptions = Array( 1 => "Less than a year", 8 => "6 - 10 years",13 => "11 - 15 years",15 => "More than 15");
		$dasheddate = str_replace('/', '-', $row->DateRegistration);
		$yearscount = array_search(trim($row->YearsSmoking), $YearsOptions);
		if (!empty($yearscount) && !empty($dasheddate)) {
			$yearofsmoking = date('Y', strtotime('-'.$yearscount.' year', strtotime($dasheddate)));	
			$row->YearsSmoking = $yearofsmoking;
		} else {
			$row->YearsSmoking = "";			
		}
    }
    //change the options of csv (Less than 5,15 - 19,More than 30,20 - 24,5 - 9,25 - 30,10 - 14) with our new option value for field_how_many_cigarettes_do_you
    if ($row->CigarettesEachDay) {
		$CigarettesOptions = Array( 5 => "Less than 5", 9 => "5 - 9",14 => "10 - 14",19 => "15 - 19",24 => "20 - 24",30 => "25 - 30",35 => "More than 30");
		$CigarettesEachDay = array_search($row->CigarettesEachDay, $CigarettesOptions);
		$row->CigarettesEachDay = $CigarettesEachDay;
    }
    //change the options of csv (Never before,Once,More than twice) with our new option value for field_have_you_used_champix_befo
    if ($row->TriedChampix) {
		if ($row->TriedChampix != "Never before") {
			$row->TriedChampix = "yes";			
		} else {
			$row->TriedChampix = "no";			
		}
    } 	  
	//change the options of csv (Once,Twice,More than 15 times,11 - 15 times,6 - 10 times,3 times) with our new option value field_how_many_times_have_you_tr
    if ($row->TimesQuitSmoking) {
		$Times_qs_options = Array( 1 => "Once", 2 => "Twice",3 => "3 times",8 => "6 - 10 times",13 => "11 - 15 times",15 => "More than 15 times");
		$TimesQuitSmoking = array_search(trim($row->TimesQuitSmoking), $Times_qs_options);
		$row->TimesQuitSmoking = $TimesQuitSmoking;
    }
	//change the options of csv (Boredom,Driving,Concentrating,Enforced no-smoking,Stress,Offered a cigarette,Anxious / nervous,Alcohol,Waking up,Completed a task,Relaxed,Coffee / Tea,With Smokers,Socialising) with our new option value field_what_triggers_you_to_smoke
    if ($row->PrimaryTriggers) {
		$PrimaryTriggersOptions = Array( 'boredom' => "Boredom", 'socialising' => "Socialising",'stress' => "Stress");		
		$Oldoptions = explode("|", $row->PrimaryTriggers);
		$PrimaryTriggers = array_intersect($PrimaryTriggersOptions,$Oldoptions);
		$PrimaryTriggers = array_flip($PrimaryTriggers);
		$row->PrimaryTriggers = $PrimaryTriggers;
    }
	//change the options of csv (Weight gain,Dealing with stress/mood changes/irritability,Failing this quit attempt,Being able to break the habit/routine,Nicotine withdrawal symptoms) with our new option value field_how_many_times_have_you_tr
    if ($row->Fears) {
		$FearsOptions = Array('failing quit attempt' => 'Failing this quit attempt','mood' => 'Dealing with stress/mood changes/irritability','weight gain' => 'Weight gain','withdrawal' => 'Nicotine withdrawal symptoms');		
		$FearsOldoptions = explode("|", $row->Fears);
		$Fears = array_intersect($FearsOptions,$FearsOldoptions);
		$Fears = array_flip($Fears);
		$row->Fears = $Fears;
    }
    
    
  }
    
  public function complete($account, $row) {
    // User save tries to convert the password into hash.
    // Plain password save https://www.drupal.org/node/1349758.
    $time = strtotime("now");
    db_update('pfe_profile_metadata')
    ->fields(array('sms_optout' => '1', 'email_optout' => '1', 'sms_optout_date' => $time, 'email_optout_date' => $time, ))
    ->condition('uid', $account->uid)
    ->execute();
  } 
    
}
