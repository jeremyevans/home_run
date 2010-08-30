require File.expand_path('../../spec_helper', __FILE__)

ruby_version_is "1.9" do
  describe "Date string encoding methods" do
    it "should return strings in US-ASCII encoding by default" do
      d = Date.today
      d.asctime.encoding.name.should == 'US-ASCII'
      d.ctime.encoding.name.should == 'US-ASCII'
      d.httpdate.encoding.name.should == 'US-ASCII'
      d.inspect.encoding.name.should == 'US-ASCII'
      d.iso8601.encoding.name.should == 'US-ASCII'
      d.jisx0301.encoding.name.should == 'US-ASCII'
      d.rfc2822.encoding.name.should == 'US-ASCII'
      d.rfc3339.encoding.name.should == 'US-ASCII'
      d.rfc822.encoding.name.should == 'US-ASCII'
      d.strftime('%S:%M:%H').encoding.name.should == 'US-ASCII'
      d.to_s.encoding.name.should == 'US-ASCII'
      d.xmlschema.encoding.name.should == 'US-ASCII'
    end

    it "should return strings in default_internal encoding if set" do
      begin
        Encoding.default_internal = 'UTF-8'
        d = Date.today
        d.asctime.encoding.name.should == 'UTF-8'
        d.ctime.encoding.name.should == 'UTF-8'
        d.httpdate.encoding.name.should == 'UTF-8'
        d.inspect.encoding.name.should == 'UTF-8'
        d.iso8601.encoding.name.should == 'UTF-8'
        d.jisx0301.encoding.name.should == 'UTF-8'
        d.rfc2822.encoding.name.should == 'UTF-8'
        d.rfc3339.encoding.name.should == 'UTF-8'
        d.rfc822.encoding.name.should == 'UTF-8'
        d.strftime('%S:%M:%H').encoding.name.should == 'UTF-8'
        d.to_s.encoding.name.should == 'UTF-8'
        d.xmlschema.encoding.name.should == 'UTF-8'
      ensure
        Encoding.default_internal = nil
      end
    end
  end
end
