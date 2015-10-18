module.exports = function (callback){
    return function (error, response, data) {
        if (error){
            return callback(error);
        }

        if (response && response.statusCode >= 400) {
            return callback(data);
        }

        callback(null, data);
    };
};
