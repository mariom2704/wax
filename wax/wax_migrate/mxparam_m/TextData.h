#pragma once


class  MXPARAMDLL TextData : public CObject
{
	public:
		TextData();
		virtual ~TextData();

		const std::string& get_bezeichnung()const;
		const std::string& get_id()const;

		void set_bezeichnung(const std::string& val);
		void set_id(const std::string& val);

	private:
		std::string id;

		std::string bezeichnung;

};


class MXPARAMDLL TextDataList	: public CObList
{
	public:
		TextDataList();
		virtual ~TextDataList();

	    static TextDataList* getInstance();
		static void dispose();

		const std::string& getText(const std::string& id, bool nilIfNotFound = false);
		std::string getTextWithColon(const std::string& id);

	private:
		
		static TextDataList* instance;
	
};

