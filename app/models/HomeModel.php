<?php

class HomeModel extends Model {

    static $table = 'table_name';

    public static function init(){        
        parent::setTable(self::$table);
        parent::setFieldId('field_id_name');
    }

}
